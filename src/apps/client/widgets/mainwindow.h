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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QtGui>
#include <QStackedWidget>
#include <QtXml/QDomDocument>

namespace Core {
    class ICore;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QString configFile, QWidget *parent = 0);
    ~MainWindow();

    void init();

    QWidget *mainWidget(QString name) const;
    void registerWidget(QString name, QWidget *widget);

private slots:
    void closeSession();
    void changePassword();

    void coreLogged();
    void coreLoggedOut();

    void mainWidgetActionTrigred(QAction * action);

protected:
    void closeEvent(QCloseEvent * event);


private:
    Core::ICore *m_core;
    QMenuBar *m_menuBar;
    QToolBar *m_tbMainWidget;
    QToolBar *m_tbMainWidgetActions;

    QAction *m_acCloseSession;
    QAction *m_acChangePassword;
    QAction *m_acClose;

    QMap<QString, QWidget *> m_widgetById;
    QMap<QString, int> m_widgetIndex;
    QStackedWidget *m_mainWidgets;
    QString m_configFile;

private:
    void setupWidgets();

    void updateWindowTitle();
    void setupWidgetActions(QString name);

    void addWidget(const QDomNode &node);

    void parseAppArgs();
    void generateMenuFromXml(const QString &xml);
    void generateMainWidgetsFromXml(const QString &xml);

    QMenu *getMenu(const QDomNode &node);


protected:



};

//} //namespace Core

#endif // MAINWINDOW_H
