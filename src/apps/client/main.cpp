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
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtDeclarative/QDeclarativeProperty>
#include <QObject>
#include <QQuickWindow>
#include <QMessageBox>
#include <QQuickPaintedItem>

#include "core.h"
#include "user.h"
#include "cryptor.h"
#include "sqlmodel.h"

QString m_configFile;
QString m_user;
QString m_pwd;

void parseAppArgs()
{
    QStringList args = QApplication::arguments();
    args.removeFirst();

    if (args.isEmpty()) {
        return;
    }

    for (int i = 0; i < args.count();) {
        QString nextArg = i+1 < args.count() ? args.at(i + 1) : QString();

        if (args.at(i) == "-c" || args.at(i) == "--config") {
            if (nextArg.isEmpty() || nextArg.startsWith("-")) {
                qDebug() << "Invalid config file";
                i++;
                continue;
            }

            if (!QFile::exists(nextArg)) {
                qDebug() << nextArg << ": config file not found";
                i += 2;
                continue;
            }

            m_configFile = nextArg;
            i += 2;
        } else if (args.at(i) == "-u" || args.at(i) == "--user") {
            if (nextArg.isEmpty() || nextArg.startsWith("-")) {
                qDebug() << "Invalid user parameters";
                i++;
                continue;
            }

            m_user = nextArg;
            i += 2;
        } else if (args.at(i) == "-p" || args.at(i) == "--password") {
            if (nextArg.isEmpty() || nextArg.startsWith("-")) {
                qDebug() << "Invalid password parameters";
                i++;
                continue;
            }

            m_pwd = nextArg;
            i += 2;
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLocale::setDefault(QLocale::system());

    app.setOrganizationName("WSoft");
    app.setApplicationName("Rubus");
#ifdef Q_OS_WIN
    app.setFont(QFont("tahoma", 10));
#endif

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

    qmlRegisterType<Core::ICore>("Rubus", 1, 0, "Core");
    qmlRegisterType<User>("Rubus", 1, 0, "User");
    qmlRegisterType<Cryptor>("Rubus", 1, 0, "Cryptor");
    qmlRegisterType<SqlModel>("Rubus", 1, 0, "SqlModel");

    QTranslator translator;
    translator.load("rubus_ru");
    app.installTranslator(&translator);

    parseAppArgs();

    Core::ICore core(m_configFile);
    Settings *sett = core.settings(QSettings::UserScope);

    QQmlApplicationEngine engine;    
    engine.setBaseUrl(QUrl(QML_BASE_DIR));
    engine.addImportPath(QML_BASE_DIR);
    engine.rootContext()->setContextProperty("core", &core);
    engine.rootContext()->setContextProperty("settings", sett);
    bool logged = false;

    QObject::connect(&core, SIGNAL(mainWindowDataLoaded(QByteArray, QUrl)), &engine, SLOT(loadData(QByteArray,QUrl)));

    if (!m_user.isEmpty() && !m_pwd.isEmpty()) {
        logged = core.login(m_user, m_pwd);

        if (!logged) {
            QMessageBox::critical(0, "Core error", core.errorString());
        }
    } else {
        m_user = sett->value("lastUser").toString();

        if (sett->value("isSave").toBool() == true) {
            m_pwd = Cryptor::decode(sett->value("pwd").toString());
            logged = core.login(m_user, m_pwd);

            if (!logged) {
                QMessageBox::critical(0, "Core error", core.errorString());
            }
        }
    }

    if (!logged) {
        engine.load(QUrl("qrc:/qml/SigninDialog.qml"));
        QObject *signin = engine.rootObjects().first();
        if (!signin) {
            return -1;
        }

        signin->setProperty("username", m_user);
        signin->setProperty("issave", sett->value("isSave").toBool());
    }

    return app.exec();
}
