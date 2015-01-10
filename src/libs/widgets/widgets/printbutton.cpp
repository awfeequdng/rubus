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
#include "printbutton.h"
#include "reportmanager.h"
#include "report.h"

#include <QMenu>
#include <QAction>

PrintButton::PrintButton(QWidget *parent) :
    QPushButton(tr("Print"),parent)
{
    m_menu = new QMenu(this);

    setAutoDefault(false);
    setEnabled(false);
    setMenu(m_menu);

    connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(onMenuTriggered(QAction*)));
    connect(this, SIGNAL(clicked()), SLOT(onClick()));
}

void PrintButton::populateMenu(QString menuId)
{
    m_menu->clear();
    qDeleteAll(m_reports);
    m_reports.clear();
    //m_reports = ReportManager::reportsByMenuId(menuId);

    QListIterator<Report*> iter(m_reports);
    while(iter.hasNext()) {
        Report *r = iter.next();

        QAction *ac = new QAction(r->name(),this);
        m_menu->addAction(ac);

        ac->setData(QVariant::fromValue(r));
    }

    setEnabled(m_reports.count() > 0);
}

void PrintButton::onMenuTriggered(QAction *ac)
{

//    Report r = static_cast<Report>(ac->data().value<Report>());

//    emit print(r);
}

void PrintButton::onClick()
{
//    if (m_reports.count() > 0) {
//        Report rep = m_reports.at(0);
//        emit print(rep);
//    }
}
