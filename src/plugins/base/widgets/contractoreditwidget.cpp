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
#include "contractoreditwidget.h"
#include "ui_contractoreditwidget.h"
#include "models/contractortypemodel.h"
#include "contractor.h"

#include <QtSql>
#include <QDebug>

ContractorEditWidget::ContractorEditWidget(QWidget *parent) :
    EditWidgetInterface(parent),
    ui(new Ui::ContractorEditWidget)
{
    ui->setupUi(this);
    m_typeModel = new ContractorTypeModel(this);
    ui->cmbType->setModel(m_typeModel, ContractorTypeModel::IdCol, ContractorTypeModel::NameCol);

    connect(ui->btnSave, SIGNAL(clicked()), SLOT(save()));
    connect(ui->btnCancel, SIGNAL(clicked()), SIGNAL(rejected()));
}

ContractorEditWidget::~ContractorEditWidget()
{
    delete ui;
}


QVariant ContractorEditWidget::id() const
{
    return m_id;
}

bool ContractorEditWidget::load(QVariant id)
{
    m_id = id.isValid() ? id.toInt() : -1;

    ui->cmbType->setCurrentKey(Contractor::Employee);
    ui->edName->clear();

    if (m_id != -1) {
        QSqlQuery sql;
        sql.exec(QString("SELECT co_name, co_type FROM contractors WHERE co_id = %1")
                 .arg(m_id));

        if (sql.lastError().isValid()) {
            setErrorString(sql.lastError().text());
            qCritical() << sql.lastError();
            return false;
        }

        if (!sql.next()) {
             setErrorString(tr("Contractore #%1 not found!").arg(m_id));
             return false;
        }

        ui->edName->setText(sql.value(0).toString());
        ui->cmbType->setCurrentKey(sql.value(1).toInt());
    }

    setWindowTitle(m_id == -1 ? tr("New contractor") : tr("Edit contractor #%1").arg(m_id));
    return true;
}

bool ContractorEditWidget::save()
{
    QSqlQuery sql;

    if (m_id == -1) {
        sql.prepare("INSERT INTO contractors (co_name, co_type) VALUES (:name, :type)");
    } else {
        sql.prepare("UPDATE contractors SET co_name = :name, co_type = :type WHERE co_id = :id");
        sql.bindValue(":id", m_id);
    }

    sql.bindValue(":name", ui->edName->text());
    sql.bindValue(":type", ui->cmbType->currentKey().toInt());

    if (!sql.exec()) {
        setErrorString(sql.lastError().text());
        qCritical() << sql.lastError();
        return false;
    }

    if (m_id == -1) {
        sql.exec("SELECT currval(pg_get_serial_sequence('contractors', 'co_id'))");

        if (sql.next()) {
            m_id = sql.value(0).toInt();
        } else {
            qWarning() << sql.lastError();
        }
    }

    emit saved();

    return true;
}
