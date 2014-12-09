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
#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QMap>

#include "core_global.h"
#include "settings.h"


class EditWidgetInterface;
class QAction;
class ReportManager;


namespace Core {

class User;
class IPlugin;
class PluginManager;

class CORE_EXPORT ICore : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(QString database READ database WRITE setDatabase NOTIFY databaseChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString version READ version CONSTANT)

public:
    explicit ICore(QString configFile = QString());

    ~ICore();

    static ICore *instance();
    static PluginManager *pluginManager();
    static ReportManager *reportManager();

    static void loadPlugins();

    Q_INVOKABLE bool login(QString username, QString password);
    Q_INVOKABLE bool logout();

    static void registerWidget(QString name, QWidget *widget);

    static void registerAction(QString id, QAction * action);
    static void registerActions(QMap<QString, QAction *> map);
    static QAction * actionById(QString id);

    QString host() const { return m_databaseHost; }
    void setHost(QString host) { m_databaseHost = host; }

    QString database() const { return m_databaseName; }
    void setDatabase(QString name) { m_databaseName = name; }

    int port() const { return m_databasePort; }
    void setPort(int port) { m_databasePort = port; }

    Q_INVOKABLE QString errorString() const { return m_errorString; }
    bool canChangeDatabaseSettings() { return m_canChangeDatabaseSettings; }

    Q_INVOKABLE static User *currentUser();
    QByteArray mainWndowQml() const { return m_mainwindowQml; }

    void loadParameters();
    void saveParameters();

    static QString version();
    Settings *settings(QSettings::Scope scope);

signals:
    void logged();
    void loggedOut();
    void hostChanged();
    void databaseChanged();
    void portChanged();
    void mainWindowDataLoaded(const QByteArray & data, const QUrl & url);

public slots:

private:
    void loadConfig();
    void saveConfig();

    QString m_databaseHost;
    QString m_databaseName;
    int m_databasePort;
    QString m_errorString;
    bool m_canChangeDatabaseSettings;
    User *m_currentUser;
    QString m_reportStoragePath;
    QString m_configFile;
    QByteArray m_mainwindowQml;

    Settings *m_systemSettings;
    Settings *m_userSettings;

    QMap<QString, QAction *> m_actionById;
};
}


#endif // CORE_H
