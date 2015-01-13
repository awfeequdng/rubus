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
#include "core.h"
#include "iplugin.h"
#include "user.h"
#include "pluginmanager.h"
#include "version.h"

#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QtSql>
#include <QAction>
#include "qjsondocument.h"
#include "qjsonobject.h"

#define CONFIG_FILENAME "rubus.conf"

using namespace Core;

static ICore *m_instance = 0;
static PluginManager *m_pluginManager = 0;
static QWidget *m_mainwindow = 0;

ICore::ICore(QWidget *mainwindow, QString configFile)
{
    m_instance = this;
    m_mainwindow = mainwindow;
    m_pluginManager = new PluginManager(this);

    loadConfig(configFile.isEmpty() ? CONFIG_FILENAME : configFile);
}

ICore::~ICore()
{
    saveConfig();
    m_instance = 0;
    delete m_pluginManager;
    m_pluginManager = 0;
}

ICore *ICore::instance()
{
    return m_instance;
}

PluginManager *ICore::pluginManager()
{
    return m_pluginManager;
}

QWidget *ICore::mainwindow()
{
    return m_mainwindow;
}

void ICore::loadPlugins()
{
    pluginManager()->loadPlugins();
}

bool ICore::login(QString username, QString password)
{
    if (username.simplified().isEmpty()) {
        m_errorString = tr("Username cannot be empty.");
        return false;
    }

    if (QSqlDatabase::database().isOpen()) {
        QSqlDatabase::database().close();
        emit loggedOut();
    }

    bool contains = QSqlDatabase::connectionNames().contains(QSqlDatabase::database().connectionName());

    QSqlDatabase db = contains ? QSqlDatabase::database() : QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(m_databaseHost);
    db.setUserName(username);
    db.setPassword(password);
    db.setDatabaseName(m_databaseName);
    db.setPort(m_databasePort);

    if (db.open()) {
        m_currentUser = new User(username, this);
        if (!m_currentUser->load()) {
            m_errorString = m_currentUser->errorString();
            return false;
        }

        loadParameters();
        emit logged();
    } else {
        qDebug() << db.lastError();
        m_errorString = db.lastError().text();
    }

    return db.isOpen();
}

bool ICore::logout()
{    
    saveParameters();
    saveConfig();

    QSqlDatabase::database().close();
    emit loggedOut();
    return true;
}

void ICore::registerWidget(QString name, QWidget *widget)
{
   m_instance->m_widgetById.insert(name, widget);
}

QWidget *ICore::widgetByName(const QString &name)
{
    return m_instance->m_widgetById.value(name, 0);
}

void ICore::registerAction(QString id, QAction *action)
{
    m_instance->m_actionById.insert(id, action);
}

void ICore::registerActions(QMap<QString, QAction *> map)
{
    QMapIterator<QString, QAction *> i(map);

    while (i.hasNext()) {
        i.next();
        registerAction(i.key(), i.value());
    }
}

QAction *ICore::actionById(QString id)
{
    return m_instance->m_actionById.value(id, 0);
}

User *ICore::currentUser()
{
    return m_instance->m_currentUser;
}

void Core::ICore::loadParameters()
{
    QListIterator<IPlugin *> iter(m_pluginManager->plugins());

    while(iter.hasNext()) {
        IPlugin *plugin = iter.next();

        QJsonObject params =  currentUser()->parameter(plugin->name());
        //plugin->setParametrs(params);

        QMapIterator<QString, QVariant> it_p(params.toVariantMap());
        while (it_p.hasNext()) {
            it_p.next();
            plugin->setProperty(it_p.key().toLocal8Bit().data(), it_p.value());
        }
    }
}

void ICore::saveParameters()
{
    QListIterator<IPlugin *> iter(m_pluginManager->plugins());

        while(iter.hasNext()) {
            IPlugin *plugin = iter.next();

            QVariantMap map;


            for(int i = 0; i < plugin->metaObject()->propertyCount(); i++) {
                QString name = plugin->metaObject()->property(i).name();

                if (name != "name" && name != "version" && name != "objectName") {
                    map.insert(plugin->metaObject()->property(i).name(),
                               plugin->metaObject()->property(i).read(plugin));
                }
            }

            if (!map.isEmpty()) {
                currentUser()->setParameter(plugin->name(), map);
            }
        }

        if (!currentUser()->save()) {
            qCritical() << currentUser()->errorString();
        }
}

QString ICore::version()
{
    return QString("%1.%2%3")
            .arg(Version::MAJOR)
            .arg(Version::MINOR)
            .arg(QString(Version::STAGE).isEmpty() ? "" : QString(" - %1").arg(Version::STAGE));
}

QByteArray ICore::scheme()
{
    QByteArray value;
    QFile f(":/scheme.json");
    if (f.open(QIODevice::ReadOnly)) {
        value = f.readAll();
        f.close();
    }

    return value;
}

void ICore::loadConfig(QString filename)
{
    m_configFile = filename;
    QSettings sett(filename,QSettings::IniFormat);
    m_databaseHost = sett.value("host","localhost").toString();
    m_databaseName = sett.value("database","rubus").toString();
    m_databasePort = sett.value("port",5432).toInt();
    m_canChangeDatabaseSettings = sett.value("canSettings",true).toBool();
}

void ICore::saveConfig()
{
    QSettings sett(m_configFile,QSettings::IniFormat);
    sett.setValue("host",m_databaseHost);
    sett.setValue("database",m_databaseName);
    sett.setValue("port",m_databasePort);
}

