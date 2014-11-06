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
#include "standardtabledialog.h"
#include "ui_standardtabledialog.h"
#include "editdialog.h"

StandardTableDialog::StandardTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StandardTableDialog),
    m_model(0),
    m_editWidget(0),
    m_editDialog(0)
{
    ui->setupUi(this);
}

StandardTableDialog::~StandardTableDialog()
{
    delete ui;
}

QAbstractItemModel *StandardTableDialog::model() const
{
    return m_model;
}

void StandardTableDialog::setModel(QAbstractItemModel *model)
{
    m_model = model;
    ui->tableView->setModel(m_model);
}

EditWidgetInterface *StandardTableDialog::editWidget() const
{

}

void StandardTableDialog::setEditWidget(EditWidgetInterface *widget)
{
    if (m_editDialog) {
        delete m_editDialog;
    }

    m_editDialog = new EditDialog()
}
