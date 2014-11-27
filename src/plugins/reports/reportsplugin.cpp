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
#include "reportsplugin.h"
#include "reportsconstants.h"
#include "reportmanager.h"
#include "widgets/reportsdialog.h"

#include <QtPlugin>
#include <QDebug>
#include <QAction>

using namespace Core;

static ReportsPlugin *m_instance = 0;

ReportsPlugin::ReportsPlugin(QObject *parent) :
    IPlugin(parent)
{
    m_instance = this;
    m_reportManager = new ReportManager(this);
}

ReportsPlugin::~ReportsPlugin()
{
    delete m_reportManager;
}


QString ReportsPlugin::name() const
{
    return "Reports";
}

int ReportsPlugin::version() const
{
    return 1;
}

bool ReportsPlugin::initialize()
{
    m_reportDlg = new ReportsDialog(ICore::mainWindow());

    m_acReportManager = new QAction(tr("Report manager"),this);
    connect(m_acReportManager, SIGNAL(triggered()), SLOT(showReportManager()));
    ICore::registerAction(Constants::A_REPORTMANAGER, m_acReportManager);


    return true;
}

void ReportsPlugin::showReportManager()
{
    m_instance->m_reportDlg->show();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Reports, ReportsPlugin)
#else
#endif

