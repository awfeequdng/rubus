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
#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QHash>


#include "core_global.h"
#include "iplugin.h"

namespace Core {

class CORE_EXPORT PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QObject *parent = 0);
    ~PluginManager();

    static PluginManager *instance();

    void loadPlugins();
    void loadPlugin(QString filename);

    IPlugin *plugin(QString pluginName) const { return m_pluginByName.value(pluginName); }
    QList<IPlugin *> plugins() const;

    QMap<QString, QAction*> pluginActions() const { return m_pluginActionById; }

signals:
    void  endInitialized();

public slots:

private:
    QHash<QString, IPlugin *> m_pluginByName;
    QHash<QString, bool> m_pluginIsInit;
    QMap<QString, QAction*> m_pluginActionById;

    void initPlugins();
    bool initPlugin(QString pluginName);

    bool pluginIsInit(QString pluginName) const { return m_pluginIsInit.value(pluginName, false); }



};

}

#endif // PLUGINMANAGER_H
