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
#ifndef RUBUSCORE_H
#define RUBUSCORE_H

#include <QObject>
#include <QMap>

#include "core_global.h"


class EditWidgetInterface;
class QAction;

namespace Core {

class User;
class MainWindow;
class IPlugin;
class PluginManager;
class ReportManager;

class CORE_EXPORT ICore : public QObject
{
    Q_OBJECT
    friend class MainWindow;
    explicit ICore(MainWindow *mainWindow, QString configFile = QString());

public:

    ~ICore();

    static ICore *instance();
    static PluginManager *pluginManager();

    static void loadPlugins();

    static MainWindow *mainWindow();
    static ReportManager *reportManager();

    bool login(QString username, QString password);
    bool logout();

    static void registerWidget(QString name, QWidget *widget);

    static void registerAction(QString id, QAction * action);
    static void registerActions(QMap<QString, QAction *> map);
    static QAction * actionById(QString id);

    QString databaseHost() const { return m_databaseHost; }
    void setDatabaseHost(QString host) { m_databaseHost = host; }

    QString databaseName() const { return m_databaseName; }
    void setDatabaseName(QString name) { m_databaseName = name; }

    int databasePort() const { return m_databasePort; }
    void setDatabasePort(int port) { m_databasePort = port; }

    QString errorString() const { return m_errorString; }
    bool canChangeDatabaseSettings() { return m_canChangeDatabaseSettings; }

    static User *currentUser();

    void loadParameters();
    void saveParameters();

    static QString version();



signals:
    void logged();
    void loggedOut();

public slots:

private:
    void loadConfig(QString filename);
    void saveConfig();

    QString m_databaseHost;
    QString m_databaseName;
    int m_databasePort;
    QString m_errorString;
    bool m_canChangeDatabaseSettings;
    User *m_currentUser;
    QString m_reportStoragePath;
    QString m_configFile;

    QMap<QString, QAction *> m_actionById;
};

}

#endif // RUBUSCORE_H
