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
#ifndef REPORTSPLUGIN_H
#define REPORTSPLUGIN_H

#include "reports_global.h"
#include "iplugin.h"

class ReportManager;
class ReportsDialog;

class REPORTS_EXPORT ReportsPlugin : public Core::IPlugin
{
    Q_OBJECT
    Q_INTERFACES(Core::IPlugin)
#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "ReportsPlugin")
#endif

public:
    explicit ReportsPlugin(QObject *parent = 0);
    ~ReportsPlugin();

    static ReportsPlugin *instance();

    QString name() const;
    int version() const;
    bool initialize();

public slots:
    static void showReportManager();

private:
    QAction *m_acReportManager;

    //ReportManager *m_reportManager;
    ReportsDialog *m_reportDlg;
};

#endif // REPORTSPLUGIN_H

