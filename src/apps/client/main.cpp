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
#include "widgets/mainwindow.h"
#include "core.h"
#include "widgets/dlgauthorise.h"

#include <QApplication>

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
    QApplication a(argc, argv);

#ifdef Q_OS_WIN
    a.setFont(QFont("tahoma", 10));
#endif

    QTranslator translator;
    translator.load("rubus_ru");
    a.installTranslator(&translator);

    parseAppArgs();
    MainWindow w(m_configFile);

    DlgAuthorise dlg;
    dlg.setUserName(m_user);
    dlg.setPassword(m_pwd);

    if ((!m_user.isEmpty() && !m_pwd.isEmpty() && dlg.authorise()) ||
            dlg.exec() == QDialog::Accepted) {
        w.show();

        m_pwd.clear();
    } else
        return 0;

    return a.exec();
}
