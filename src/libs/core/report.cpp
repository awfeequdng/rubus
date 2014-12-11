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

//CuteReport
//#include "cutereport.h"
//#include "reportcore.h"
//#include "reportinterface.h"
//#include "reportpreview.h"
//#include "../plugins/standard/core_plugins/storage/sql/sql.h"

Report::Report(QObject *parent) :
    QObject(parent),
    m_id(-1)
{
}

void Report::setId(int id)
{
    m_id = id;
}

int Report::id() const
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

bool Report::isValid() const
{
    return m_id > 0;
}
