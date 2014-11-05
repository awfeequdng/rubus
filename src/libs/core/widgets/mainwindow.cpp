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
#include "mainwindow.h"
#include "user.h"
#include "core.h"
#include "coreconstants.h"
#include "imainwidget.h"
#include "pluginmanager.h"
#include "reportmanager.h"

#include "qjsondocument.h"
#include <QToolBar>
#include <QtXml>
#include <QMenuBar>
#include <QApplication>

using namespace Core;


MainWindow::MainWindow(QString configFile, QWidget *parent) :
    QMainWindow(parent),
    m_core(new ICore(this, configFile)),
    m_configFile(configFile)
{
    QLocale::setDefault(QLocale::system());

    QCoreApplication::setOrganizationName("WSoft");
    QCoreApplication::setOrganizationDomain("http://xdbm.com");
    QCoreApplication::setApplicationName("Rubus");

    setObjectName("MainWindow");

    init();

    m_core->loadPlugins();
    registerMainWidgets();


    connect(m_core, SIGNAL(logged()),   SLOT(coreLogged()));
    connect(m_core, SIGNAL(loggedOut()),SLOT(coreLoggedOut()));

    QSettings sett;
    restoreGeometry(sett.value("MainGeometry").toByteArray());
    restoreState(sett.value("MainState").toByteArray());
}

MainWindow::~MainWindow()
{
    delete m_core;

    QSettings sett;
    sett.setValue("MainGeometry",saveGeometry());
    sett.setValue("MainState", saveState());
}

void MainWindow::init()
{
    m_menuBar = new QMenuBar(this);

    m_tbMainWidget = new QToolBar(this);
    m_tbMainWidget->setObjectName("MainWidgetToolbar");
    m_tbMainWidgetActions = new QToolBar(this);
    m_tbMainWidgetActions->setObjectName("MainWidgetActionToolbar");

    m_tbMainWidget->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_tbMainWidget->setIconSize(QSize(46,46));
    m_tbMainWidget->setMovable(false);

    m_tbMainWidgetActions->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_tbMainWidgetActions->setIconSize(QSize(32,32));

    addToolBar(Qt::LeftToolBarArea,m_tbMainWidget);
    addToolBar(Qt::TopToolBarArea,m_tbMainWidgetActions);
    m_tbMainWidget->setVisible(false);
    m_tbMainWidgetActions->setVisible(false);

    connect(m_tbMainWidget,SIGNAL(actionTriggered(QAction*)),
            SLOT(mainWidgetActionTrigred(QAction*)));

    setMenuBar(m_menuBar);

    m_mainWidgets = new QStackedWidget(this);
    setCentralWidget(m_mainWidgets);
}

IMainWidget *MainWindow::mainWidget(QString name) const
{
    return m_widgetById.value(name, 0);
}

void MainWindow::closeSession()
{
    //TODO discription this    
    m_core->logout();

}

void MainWindow::changePassword()
{
    //TODO discription this
}

void MainWindow::coreLogged()
{
    updateWindowTitle();
    setupWidgets();
    populateMenu();

    if (m_tbMainWidget->actions().count() > 0) {
        setupWidgetActions(m_tbMainWidget->actions().at(0)->data().toString());
    }
}

void MainWindow::coreLoggedOut()
{
    qDebug() << "logged out";
}

void MainWindow::mainWidgetActionTrigred(QAction *action)
{
    QString name = action->data().toString();
    m_mainWidgets->setCurrentIndex(m_widgetIndex.value(name));

    setupWidgetActions(name);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QApplication::closeAllWindows();
    closeSession();

    QMainWindow::closeEvent(event);
}

void MainWindow::populateMenu()
{
    m_acCloseSession = new QAction(tr("Close session"),this);
    m_acCloseSession->setShortcut(Qt::ALT + Qt::Key_F3);
    connect(m_acCloseSession,SIGNAL(triggered()),SLOT(closeSession()));

    m_acClose = new QAction(tr("Close"),this);
    m_acClose->setShortcut(Qt::ALT + Qt::Key_F4);
    connect(m_acClose, SIGNAL(triggered()), SLOT(close()));

    QAction *aboutQt = new QAction(tr("About Qt..."),this);
    aboutQt->setMenuRole(QAction::AboutQtRole);
    connect(aboutQt,SIGNAL(triggered()),qApp, SLOT(aboutQt()));

    registerAction("close_session", m_acCloseSession);
    registerAction("close",m_acClose);
    registerActions(PluginManager::instance()->pluginActions());



    //m_pluginActionById

    generateMenuFromXml(m_core->currentUser()->gui());
}

void MainWindow::setupWidgets()
{
    m_tbMainWidget->clear();
    m_tbMainWidget->setVisible(false);
    m_widgetIndex.clear();

    generateMainWidgetsFromXml(m_core->currentUser()->gui());
    m_tbMainWidget->setVisible(m_mainWidgets->count() > 1);
}

void MainWindow::updateWindowTitle()
{
    setWindowTitle(tr("Rubus v%1 | %2")
                   .arg(Core::ICore::version())
                   .arg(m_core->currentUser()->name()));


}

void MainWindow::setupWidgetActions(QString name)
{
    m_tbMainWidgetActions->setVisible(false);
    m_tbMainWidgetActions->clear();


    QListIterator<QAction*> iter(m_widgetById.value(name)->toolBarActions());
    bool hasActions = iter.hasNext();

    while (iter.hasNext()) {
        QAction *act = iter.next();
        m_tbMainWidgetActions->addAction(act);
    }

    m_tbMainWidgetActions->setVisible(hasActions);
}

void MainWindow::addWidget(const QDomNode &node)
{
    QString name = node.toElement().attribute("id");
    IMainWidget *mwidget = m_widgetById.value(name, 0);

    if (mwidget) {
        QWidget *wdg = mwidget->widget();
        if (!wdg) {
            qCritical() << "MainWindowPlugin " << name << "returned a not valid widget!";
            return;
        }
        QString title = node.toElement().text();
        if (title.isEmpty()) {
            title = mwidget->name();
        }

        int index = m_mainWidgets->addWidget(wdg);
        QAction *ac = new QAction(mwidget->icon(), title, this);
        ac->setData(name);
        ac->setToolTip(title);
        m_tbMainWidget->addAction(ac);
        m_widgetIndex.insert(name, index);
    }
}

void MainWindow::registerMainWidgets()
{
    QListIterator<IPlugin *> iter(m_core->pluginManager()->plugins());

    while (iter.hasNext()) {
        IMainWidget * wdg = static_cast<IMainWidget *>(iter.next());
        if (wdg) {
            registerMainWidget(wdg);
        }
    }
}

void MainWindow::registerMainWidget(IMainWidget *widget)
{
    m_widgetById.insert(widget->name(), widget);
}

void MainWindow::registerAction(QString id, QAction *action)
{
    m_pluginActionById.insert(id, action);
}

void Core::MainWindow::registerActions(QMap<QString, QAction *> map)
{
    QMapIterator<QString, QAction *> i(map);

    while (i.hasNext()) {
        i.next();
        registerAction(i.key(), i.value());
    }
}

void MainWindow::generateMenuFromXml(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) {
        qDebug() << "Can't set content menu settings";
        return;
    }

    QDomElement mainmenu = doc.firstChild().toElement().firstChildElement("mainmenu");

    QDomNodeList list = mainmenu.childNodes();
    for (int i = 0; i < list.count(); i++) {
        QMenu *menu = getMenu(list.at(i));

        if (menu) {
            m_menuBar->addMenu(menu);
        }
    }
}

void MainWindow::generateMainWidgetsFromXml(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) {
        qDebug() << "Can't set content main widgets settings";
        return;
    }

    QDomElement widgets = doc.firstChild().toElement().firstChildElement("widgets");

    QDomNodeList list = widgets.childNodes();
    for (int i = 0; i < list.count(); i++) {
        addWidget(list.at(i));
    }
}

QMenu *Core::MainWindow::getMenu(const QDomNode &node)
{
    if (node.nodeName() != "menu") {
        return 0;
    }

    QDomElement el = node.toElement();
    QMenu *menu = new QMenu(el.attribute("title", "!undef"), this);

    QDomNodeList list = node.childNodes();

    for (int i = 0; i < list.count(); i++) {
        QString name = list.at(i).nodeName();
        if (name == "menu") {
            menu->addMenu(getMenu(list.at(i)));
        } else if (name == "action") {
            QDomElement el = list.at(i).toElement();
            QAction *action = m_pluginActionById.value(el.attribute("id"), 0);

            if (action) {
                if (!el.text().isEmpty()) {
                    action->setText(el.text());
                }
                menu->addAction(action);
            } else {
                qDebug() << "Action " << el.attribute("id") << " is not found";
            }
        } else if (name == "separator") {
            menu->addSeparator();
        }
    }

    return menu;
}
