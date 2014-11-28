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
#include "usermodel.h"

#include <QtSql>

UserModel ::UserModel(QObject *parent) :
    AdvItemModel(parent)
{
}

bool UserModel::populate()
{
    emit beginResetModel();

    m_removedIds.clear();
    qDeleteAll(m_items);
    m_items.clear();

    QSqlQuery sql;
    sql.exec("SELECT up_role,up_name FROM user_params");

    if (sql.lastError().isValid()) {
        qCritical() << sql.lastError();
        setLastError(sql.lastError());
        return false;
    }

    while(sql.next()) {
        UserInternal::Item *item = new UserInternal::Item;
        item->role= sql.value(0).toString();
        item->name = sql.value(1).toString();

        m_items.append(item);
    }

    emit endResetModel();
    return true;
}


int UserModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

int UserModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant UserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    UserInternal::Item *item = m_items[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch(index.column()) {;
        case RoleCol : return item->role;
        case NameCol : return item->name;
        }
    }

    return AdvItemModel::data(index, role);
}

QVariant UserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch(section) {;
            case RoleCol : return tr("Role");
            case NameCol : return tr("Name");
            }
        }
        if (role == Qt::TextAlignmentRole) {
            switch(section) {;
            case RoleCol : return Qt::AlignLeft + Qt::AlignVCenter;
            case NameCol : return Qt::AlignLeft + Qt::AlignVCenter;
            }
        }
    }

    return AdvItemModel::headerData(section, orientation, role);
}

bool UserModel::removeRows(int row, int count, const QModelIndex &parent)
{
    emit beginRemoveRows(parent, row, row + count -1);

    if (!m_removedIds.isEmpty()) {
        m_removedIds.append(",");
    }
    m_removedIds += "'" + m_items[row]->role + "'";
    m_items.removeAt(row);
    emit endRemoveRows();

    return true;
}

bool UserModel::submit()
{
    if (!m_removedIds.isEmpty()) {
        QSqlQuery sql;
        sql.exec(QString("DELETE FROM user_params WHERE up_role IN (%1)").arg(m_removedIds));

        if (sql.lastError().isValid()) {
            setLastError(sql.lastError());
            qCritical() << sql.lastError();
            return false;
        }

        m_removedIds.clear();

    }

    return true;
}

