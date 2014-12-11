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
#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include <QObject>
#include <QVariant>

#include "core_global.h"
#include "report.h"


QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

class OOoReportBuilder;
class NCReportSource;
class NCReport;

namespace CuteReport {
class ReportCore;
}

class CORE_EXPORT ReportManager : public QObject
{
    Q_OBJECT
public:
    explicit ReportManager(QObject *parent = 0);

    static ReportManager *instance();
    Q_INVOKABLE static Report & loadReport(int id);

    static void registerMenuId(QString id, QString title);
    Q_INVOKABLE static void showReport(const Report &rep);
    Q_INVOKABLE void printReport(const Report &rep, QString printerName, int copies, bool showDialog = false);

    //Q_INVOKABLE static QList<Report> reportsByMenuId(QString menuId);
#ifdef NCREPORT
    static NCReportSource reportDatabaseSource(int reportId);
#endif
signals:

private slots:

private:
    QHash<QString, QString> m_menus;

};


#endif // REPORTMANAGER_H
