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
#include "reportmanager.h"

//OpenOffice
#include "oooreportbuilder.h"

//CuteReport
//#include "cutereport.h"
//#include "reportcore.h"
//#include "reportinterface.h"
//#include "reportpreview.h"
//#include "../plugins/standard/core_plugins/storage/sql/sql.h"

//NcReport
#ifdef NCREPORT
#include "ncreport.h"
#include "ncreportoutput.h"
#include "ncreportpreviewoutput.h"
#include "ncreportpreviewwindow.h"
#include "ncreportprinteroutput.h"
#include "ncreportsource.h"
#endif

#include <QAbstractItemModel>
#include <QtSql>
#include <QSettings>

//using namespace Core;

static ReportManager *m_instance = 0;
static CuteReport::ReportCore *m_cuteReport = 0;

ReportManager::ReportManager(QObject *parent) :
    QObject(parent)
{
    m_instance = this;
    //qRegisterMetaType<Report>("Report");
}

ReportManager *ReportManager::instance()
{
    return m_instance;
}

Report &ReportManager::loadReport(int id)
{
//    QSqlQuery sql;
//    Report rep;
//    sql.exec(QString("SELECT re_name, re_type,re_menu FROM reports WHERE re_id = %1")
//             .arg(id));

//    if (sql.lastError().isValid()) {
//        qCritical() << sql.lastError();
//        return rep;
//    }
//    if (!sql.next()) {
//        qCritical() << "report not found";
//    }

//    rep = Report(sql.value(1).toInt());
//    rep.setId(id);
//    rep.setMenuId(sql.value(2).toString());
//    rep.setTitle(sql.value(0).toString());

//    return rep;
}

void ReportManager::registerMenuId(QString id, QString title)
{
    instance()->m_menus.insert(id,title);
}

//QList<Report> ReportManager::reportsByMenuId(QString menuId)
//{
//    QSqlQuery sql;
//    sql.exec(QString("SELECT re_id,re_name,re_type,re_note "
//                     "FROM reports "
//                     "WHERE re_menu = '%1'")
//             .arg(menuId));

//    QList<Report> reps;

//    if (sql.lastError().isValid()) {
//        qWarning() << sql.lastError();
//        return reps;
//    }

//    while (sql.next()) {
//        Report rep(sql.value(2).toInt());
//        rep.setId(sql.value(0).toInt());
//        rep.setTitle(sql.value(1).toString());
//        rep.setMenuId(menuId);

//        reps.append(rep);
//    }

//    return reps;
//}

void ReportManager::showReport(const Report &rep)
{
    if (rep.engine() == Report::OpenOfficeEngine) {
        OOoReportBuilder builder;
        builder.setModels(rep.models());
        builder.setParameters(rep.paramentrs());
        builder.setOutputDir(QDir::tempPath());
        builder.loadReportFile(rep.filename());
        builder.parse(true);
        return;
    }

#ifdef NCREPORT
    if (rep.engine() == Report::NcReportEngine) {
        NCReport report;
        report.setReportSource(reportDatabaseSource(rep.id()));
        NCReportOutput *output= new NCReportPreviewOutput;
        output->setAutoDelete(false);
        report.setOutput(output);

        int i = 0;
        QListIterator<QAbstractItemModel*> iter(rep.models());
        while (iter.hasNext()) {
            i++;
            QAbstractItemModel *model = iter.next();
            report.addItemModel(model, QString("model%1").arg(i));
        }

        QHashIterator<QString,QVariant> parms(rep.paramentrs());
        while(parms.hasNext()) {
            parms.next();
            report.addParameter(parms.key(),parms.value());
        }

        report.runReport();

        if (!report.hasError()){
            NCReportPreviewWindow *pv = new NCReportPreviewWindow();
            pv->setOutput((NCReportPreviewOutput*)report.output());
            pv->setWindowModality( Qt::ApplicationModal);
            pv->setAttribute( Qt::WA_DeleteOnClose );
            pv->show();
        } else {
            qCritical() << report.lastErrorMsg();
        }

        return;
    }
#endif


//    if (rep.engine() == Report::CuteReportEngine) {
//        if (!m_cuteReport) {
//            QSettings sett;
//            if (sett.value("CuteReport/PluginsPath").toString().isEmpty()) {
//                sett.setValue("CuteReport/PluginsPath", CUTEREPORT_BUILD_PLUGINS);
//            }

//            m_cuteReport =  new CuteReport::ReportCore(&sett);
//            StorageSql *storage = static_cast<StorageSql*>(m_cuteReport->storageModule("Standard::SQL"));
//            if (storage) {
//                //storage->setConnectionId(QSqlDatabase::database().connectionName());
//                //storage->setTableName("reports");
//                //storage->setColumnData("re_data");
//                //storage->setColumnId("re_id");
//                m_cuteReport->setDefaultStorage("Standard::SQL");
//            } else {
//                qCritical() << "can't cast to StorageSql";
//                return;
//            }
//        }

//        QString err;
//        CuteReport::ReportInterface * report = m_cuteReport->loadReport(QString("sql:<%1>").arg(rep.id()), &err);

//        if (!report) {
//            qDebug() << QString("Can't load report %1, error: ").arg(rep.filename()) << err;
//            return;
//        }

//        int i = 0;
//        QListIterator<QAbstractItemModel*> iter(rep.models());
//        while (iter.next()) {
//            i++;
//            QAbstractItemModel *model = iter.next();
//            report->setVariableValue(QString("model%1").arg(i), qlonglong(model));
//        }


//        report->setVariables(rep.paramentrs());


//        CuteReport::ReportPreview * preview = new CuteReport::ReportPreview(m_cuteReport);
//        if (report) {
//            preview->setReportCore(m_cuteReport);
//            preview->connectReport(report);
//            preview->showMaximized();

//            preview->run();
//        }
//    }
}

void ReportManager::printReport(const Report &rep, QString printerName, int copies,  bool showDialog)
{
#ifdef NCREPORT
    if (rep.engine() == Report::NcReportEngine) {

        NCReport report;
        report.setReportSource(reportDatabaseSource(rep.id()));

        NCReportOutput *output = new NCReportPrinterOutput();
        output->setAutoDelete(false);
        report.setOutput(output);

        int i = 0;
        QListIterator<QAbstractItemModel*> iter(rep.models());
        while (iter.next()) {
            i++;
            QAbstractItemModel *model = iter.next();
            report.addItemModel(model, QString("model%1").arg(i));
        }

        QHashIterator<QString,QVariant> parms(rep.paramentrs());
        while(parms.hasNext()) {
            parms.next();
            report.addParameter(parms.key(),parms.value());
        }

        if (!report.hasError()){
            report.runReportToPrinter(copies, showDialog, 0, printerName);
        } else {
            qCritical() << report.lastErrorMsg();
        }

        return;
    }
#endif
}

#ifdef NCREPORT
NCReportSource ReportManager::reportDatabaseSource(int reportId)
{
    NCReportSource rs;
    rs.setSourceType(NCReportSource::Database);
    rs.setConnectionID(QSqlDatabase::database().connectionName());
    rs.setTableName("reports");
    rs.setColumnName("re_data");
    rs.setKeyColumnName("re_id");
    rs.setKeyValue(QString::number(reportId));

    return rs;
}
#endif
