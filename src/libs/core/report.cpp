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
#include "report.h"

#ifdef CUTEREPORT
#include "cutereport.h"
#include "reportcore.h"
#include "reportinterface.h"
#include "reportpreview.h"
#include "storageinterface.h"
#endif

#include <QtSql>


#ifdef CUTEREPORT
static CuteReport::ReportCore *m_cuteReport = 0;
#endif

Report::Report(QObject *parent) :
    QObject(parent),
    m_id(-1)
{
}

void Report::setReportId(int id)
{
    m_id = id;
}

int Report::reportId() const
{
    return m_id;
}

QString Report::name() const
{
    return m_name;
}

void Report::setName(QString title)
{
    m_name = title;
}

QString Report::menu() const
{
    return m_menuId;
}

void Report::setMenu(QString menu)
{
    m_menuId = menu;
}

void Report::appendModel(QAbstractItemModel *model)
{
    m_models.append(model);
}

QAbstractItemModel *Report::modelAt(int i) const
{
    return m_models.at(i);
}

QList<QAbstractItemModel *> Report::models() const
{
    return m_models;
}

void Report::clearModels()
{
    m_models.clear();
}

void Report::appendParametr(QString name, QVariant value)
{
    m_params.insert(name, value);
}

QVariant Report::parametrValue(QString paramName) const
{
    return m_params.value(paramName);
}

QHash<QString, QVariant> Report::paramentrs() const
{
    return m_params;
}

bool Report::load()
{
    if (m_id > 0) {
        QSqlQuery sql;
        sql.exec(QString("SELECT re_name, re_menu FROM reports WHERE re_id = %1").arg(m_id));

        if (sql.lastError().isValid()) {
            setError(sql.lastError().text());
            return false;
        }

        sql.next();
        setName(sql.value(0).toString());
        setMenu(sql.value(1).toString());
    } else {
        m_id = -1;
    }

    emit loaded();
    return true;
}

bool Report::save()
{
    QSqlQuery sql;
    if (m_id == -1) {
        sql.prepare("INSERT INTO reports (re_name, re_menu, re_data) "
                    "VALUES (:name, :menu, :data)");
    } else {
        sql.prepare("UPDATE reports SET re_name = :name, "
                    "re_menu = :menu, "
                    "re_data = :data "
                    "WHERE re_id = :id");
        sql.bindValue(":id", m_id);
    }

    sql.bindValue(":name", m_name);
    sql.bindValue(":menu", m_menuId);
    sql.bindValue(":data", QString());

    if (!sql.exec()) {
        qCritical() << sql.lastError();
        setError(sql.lastError().text());
        return false;
    }

    emit saved();
    return true;
}

bool Report::isValid() const
{
    return m_id > 0;
}

void Report::show()
{
#ifdef CUTEREPORT
    if (!m_cuteReport) {
        QSettings sett;
        if (sett.value("CuteReport/PluginsPath").toString().isEmpty()) {
            sett.setValue("CuteReport/PluginsPath", CUTEREPORT_BUILD_PLUGINS);
        }

        m_cuteReport =  new CuteReport::ReportCore(&sett);
        CuteReport::StorageInterface *storage = static_cast<CuteReport::StorageInterface*>(m_cuteReport->storage("Standard::SQL"));
        if (storage) {
            storage->setProperty("connectionId", QSqlDatabase::database().connectionName());
            storage->setProperty("tableName", "reports");
            storage->setProperty("columnData","re_data");
            storage->setProperty("columnId", "re_id");
            m_cuteReport->setDefaultStorage("Standard::SQL");
        } else {
            setError(QString("can't cast to StorageSql"));
            return;
        }
    }

    QString err;
    CuteReport::ReportInterface * report = m_cuteReport->loadReport(QString("sql://<%1>").arg(reportId()), &err);

    if (!report) {
        setError(QString("Can't load report %1, error: ").arg(name()) + err);
        return;
    }

    int i = 0;
    QListIterator<QAbstractItemModel*> iter(models());
    while (iter.next()) {
        i++;
        QAbstractItemModel *model = iter.next();
        report->setVariableValue(QString("model%1").arg(i), qlonglong(model));
    }


    report->setVariables(paramentrs());


    CuteReport::ReportPreview * preview = new CuteReport::ReportPreview(m_cuteReport);
    if (report) {
        preview->setReportCore(m_cuteReport);
        preview->connectReport(report);
        preview->showMaximized();

        preview->run();
    }
#endif
}

void Report::print(QString printerName, int copies, bool showDialog)
{
    Q_UNUSED(printerName)
    Q_UNUSED(copies)
    Q_UNUSED(showDialog)
}

QString Report::errorString() const
{
    return m_errorString;
}

void Report::setError(const QString &error)
{
    m_errorString = error;
    emit errorStringChanged();
}
