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
#include "base.h"
#include "constants.h"
#include "widgets/standardtabledialog.h"

#include <QtPlugin>
#include <QAction>
#include <QDebug>

using namespace Core;

static Base *m_instance = 0;

Base::Base(QObject *parent) :
    IPlugin(parent)
{
    m_instance = this;
}


QString Base::name() const
{
    return "Base";
}

int Base::version() const
{
    return 1;
}

bool Base::initialize()
{
    m_acUserManager = new QAction(tr("Users"),this);
    connect(m_acUserManager, SIGNAL(triggered()), SLOT(showUserManager()));
    ICore::registerAction(Constants::A_USERMANAGER, m_acUserManager);

    m_acReportManager = new QAction(tr("Report manager"),this);
    connect(m_acReportManager, SIGNAL(triggered()), SLOT(showReportManager()));
    ICore::registerAction(Constants::A_REPORTMANAGER, m_acReportManager);

    m_acLocations = new QAction(tr("Locations"),this);
    connect(m_acLocations, SIGNAL(triggered()), SLOT(showLocationTable()));
    ICore::registerAction(Constants::A_LOCATIONS, m_acLocations);

    m_acContractors = new QAction(tr("Contractors"),this);
    connect(m_acContractors, SIGNAL(triggered()), SLOT(showContractorTable()));
    ICore::registerAction(Constants::A_CONTRACTORS, m_acContractors);

    m_acItems = new QAction(tr("Items"),this);
    connect(m_acItems, SIGNAL(triggered()), SLOT(showItemTable()));
    ICore::registerAction(Constants::A_ITEMS, m_acItems);

    m_itemDialog = new StandardTableDialog();
    m_itemDialog->setWindowTitle(tr("Items"));
    m_itemDialog->setReportMenu(Constants::A_ITEMS);

    return true;
}

void Base::showUserManager()
{
    qDebug() << "users";
}

void Base::showReportManager()
{
    qDebug() << "reprots";
}

void Base::showLocationTable()
{

}

void Base::showContractorTable()
{

}

void Base::showItemTable()
{
    m_instance->m_itemDialog->show();
    m_instance->m_itemDialog->activateWindow();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Base, Base)
#else
#endif
