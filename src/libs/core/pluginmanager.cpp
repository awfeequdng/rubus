/***************************************************************************
 *   This file is part of the Rubus project                                *
 *   Copyright (C) 2012-2014 by Ivan Volkov                                *
 *   wulff007@gmail.com                                                    *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#include "pluginmanager.h"

#include <QPluginLoader>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "iplugin.h"

using namespace Core;

static PluginManager *__instance = 0;

PluginManager::PluginManager(QObject *parent) :
    QObject(parent)
{
    __instance = this;
}

PluginManager::~PluginManager()
{
    qDeleteAll(m_pluginByName.values());

}

PluginManager *PluginManager::instance()
{
    return __instance;
}

void PluginManager::loadPlugins()
{
    QStringList dirs;
    dirs << "plugins";

    QFileInfoList files;
    foreach (const QString & dirStr, dirs) {
        QDir dir(dirStr);
        files += dir.entryInfoList(QDir::Files);
    }

    foreach(const QFileInfo & fileName, files) {
        loadPlugin(fileName.absoluteFilePath());
    }

    m_pluginActionById.clear();

    initPlugins();
}

void PluginManager::loadPlugin(QString filename)
{
    QPluginLoader loader;
    IPlugin *plugin = 0;

    loader.setFileName(filename);
    if (!loader.load()) {
        qWarning() << "Can't load file: " << loader.errorString();
    } else {
        plugin = qobject_cast<IPlugin*>(loader.instance());
        if (plugin) {
            m_pluginByName.insert(plugin->name(), plugin);
            qDebug() <<"Plugin loaded:" << plugin->name() << "version:" << plugin->version();
        } else {
            qWarning() << "Error while loading plugin: " + loader.errorString();
        }
    }
}

QList<IPlugin *> PluginManager::plugins() const
{
    return m_pluginByName.values();
}

void PluginManager::initPlugins()
{
    QList<IPlugin *> plugs = m_pluginByName.values();
    IPlugin *plugin;

    foreach (plugin, plugs) {
        initPlugin(plugin->name());
    }

    emit endInitialized();
}

bool PluginManager::initPlugin(QString pluginName)
{
    if (m_pluginIsInit.contains(pluginName)) {
        return m_pluginIsInit.value(pluginName);
    }

    IPlugin *plug = m_pluginByName.value(pluginName);

    if (!plug) {
        qDebug() << "In init plugin 0";
        return false;
    }

    QList<QString> deps = plug->dependencyPlugins();

    bool allDepsLoaded = true;
    if (!deps.isEmpty()) {

        QString depName;
        foreach(depName, deps) {
            allDepsLoaded = initPlugin(depName);

            if (!allDepsLoaded)
                break;
        }
    }

    if (allDepsLoaded) {
        bool ok = plug->initialize();
        qDebug() << plug->name() << "initialized ok:" << ok;
        m_pluginIsInit.insert(plug->name(), ok);
        QMapIterator<QString, QAction*> iter(plug->actions());
        while (iter.hasNext()) {
            iter.next();
            m_pluginActionById.insert(iter.key(),iter.value());
        }

        return ok;
    }

    return false;
}
