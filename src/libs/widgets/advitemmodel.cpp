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
#include "advitemmodel.h"

#include <QSqlError>

AdvItemModel::AdvItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

bool AdvItemModel::populate()
{
    return false;
}


QModelIndex AdvItemModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex AdvItemModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

QVariant AdvItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {
        return headerData(index.column(), Qt::Horizontal, role);
    }

    return QVariant();
}

QString AdvItemModel::errorString() const
{
    return m_lastError;
}

void AdvItemModel::setErrorString(const QString &error)
{
   m_lastError = error;
}

void AdvItemModel::setLastError(const QSqlError &error)
{
    m_lastError = error.text();
}
