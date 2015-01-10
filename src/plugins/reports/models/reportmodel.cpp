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
#include "reportmodel.h"
#include <report.h>

#include <QtSql>

ReportModel::ReportModel(QObject *parent) :
    AdvItemModel(parent)
{
    //m_typeTitleById.insert(Report::OpenOfficeEngine, tr("OO Calc"));
    //m_typeTitleById.insert(Report::NcReportEngine, tr("NcReport"));
    //m_typeTitleById.insert(Report::CuteReportEngine, tr("CuteReport"));
}

bool ReportModel::populate()
{
    emit beginResetModel();
    qDeleteAll(m_items);
    m_items.clear();

    QSqlQuery sql;
    sql.exec("SELECT re_id,re_name,re_type,re_menu FROM reports");

    if (sql.lastError().isValid()) {
        setLastError(sql.lastError());
        emit endResetModel();
        return false;
    }

    while (sql.next()) {
        Item *item = new Item;
        item->id = sql.value(0).toInt();
        item->name = sql.value(1).toString();
        item->type = sql.value(2).toInt();
        item->menu = sql.value(3).toString();

        m_items.append(item);
    }

    emit endResetModel();
    return true;
}

int ReportModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

int ReportModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant ReportModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    Item *item = m_items.at(index.row());

    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case IdCol : return item->id;
        case NameCol : return item->name;
        case TypeCol : return m_typeTitleById.value(item->type);
        case MenuCol : return item->menu;
        }
    }

    return AdvItemModel::data(index, role);
}

QVariant ReportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch(section) {
            case IdCol : return tr("Id");
            case NameCol : return tr("Name");
            case TypeCol : return tr("Type");
            case MenuCol : return tr("Menu");
            }
        }

        if (role == Qt::TextAlignmentRole) {
            switch(section) {
            case IdCol : return Qt::AlignCenter;
            case NameCol : return Qt::AlignVCenter + Qt::AlignLeft;
            case TypeCol : return Qt::AlignVCenter + Qt::AlignLeft;
            case MenuCol : return Qt::AlignVCenter + Qt::AlignLeft;
            }
        }
    }

    return AdvItemModel::headerData(section, orientation, role);
}

