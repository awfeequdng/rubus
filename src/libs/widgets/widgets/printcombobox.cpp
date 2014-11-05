/***************************************************************************
 *   This file is part of the Xdbm project                                 *
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
#include "printcombobox.h"

#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

PrintComboBox::PrintComboBox(QWidget *parent) :
    AdvComboBox(parent)
{
    m_model = new QSqlQueryModel(this);
}

void PrintComboBox::populate(QString menu)
{
    m_model->setQuery(QString("SELECT re_id, re_name FROM reports WHERE re_menu = '%1'")
                      .arg(menu));

    if (m_model->lastError().isValid()) {
        qWarning() << m_model->lastError();
    }

    setModel(m_model, 0, 1);

    setCurrentIndex(-1);
}
