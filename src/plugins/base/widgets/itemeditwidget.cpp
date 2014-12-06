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
#include "itemeditwidget.h"
#include "ui_itemeditwidget.h"
#include "models/itemtypemodel.h"

#include <QtSql>
#include <QMessageBox>

ItemEditWidget::ItemEditWidget(QWidget *parent) :
    EditWidgetInterface(parent),
    ui(new Ui::ItemEditWidget),
    m_id(-1)
{
    ui->setupUi(this);

    m_typeModel = new ItemTypeModel(this);
    ui->cmbType->setModel(m_typeModel, ItemTypeModel::IdCol, ItemTypeModel::NameCol);

    m_unitModel = new QSqlQueryModel(this);

    connect(ui->btnSave, SIGNAL(clicked()), SLOT(save()));
    connect(ui->btnCancel, SIGNAL(clicked()), SIGNAL(rejected()));
}

ItemEditWidget::~ItemEditWidget()
{
    delete ui;
}

bool ItemEditWidget::load(QVariant id)
{
    m_id = id.isValid() ? id.toInt() : -1;

    m_unitModel->setQuery(QString("SELECT un_id, un_name FROM units ORDER BY un_name"));
    if (m_unitModel->lastError().isValid()) {
        qCritical() << m_unitModel->lastError();
        setErrorString(m_unitModel->lastError().text());
        return false;
    }

    ui->cmbUnit->setModel(m_unitModel, 0, 1);

    ui->edName->setText(QString());
    ui->edDesc->setText(QString());
    ui->edArticle->setText(QString());
    ui->cmbUnit->setCurrentIndex(0);
    ui->cmbType->setCurrentIndex(0);
    ui->ckActive->setChecked(true);

    if (m_id != -1) {
        QSqlQuery sql;
        sql.exec(QString("SELECT it_name,it_desc,it_article,"
                         "it_unit,it_type,it_active FROM items "
                         "WHERE it_id = %1")
                 .arg(m_id));

        if (sql.lastError().isValid()) {
            setErrorString(sql.lastError().text());
            qCritical() << sql.lastError();
            return false;
        }

        if (!sql.next()) {
            setErrorString(tr("Item not found!"));
            return false;
        }

        ui->edName->setText(sql.value(0).toString());
        ui->edDesc->setText(sql.value(1).toString());
        ui->edArticle->setText(sql.value(2).toString());
        ui->cmbUnit->setCurrentKey(sql.value(3).toString());
        ui->cmbType->setCurrentKey(sql.value(4).toString());
        ui->ckActive->setChecked(sql.value(5).toBool());
    }

    setWindowTitle(m_id != -1 ? tr("Edit item #%1").arg(m_id) : tr("New item"));
    return true;
}

bool ItemEditWidget::save()
{
    QSqlQuery sql;
    if (m_id == -1) {
        sql.prepare("INSERT INTO items(it_name,it_desc,it_article,"
                    "it_unit,it_type,it_active) "
                    "VALUES (:name,:desc,:article,"
                    ":unit,:type,:active)");
    } else {
        sql.prepare("UPDATE items SET it_name = :name,it_desc = :desc,it_article = :article,"
                    "it_unit = :unit,it_type = :type,it_active = :active "
                    "WHERE it_id = :id");
        sql.bindValue(":id", m_id);
    }

    sql.bindValue(":name", ui->edName->text());
    sql.bindValue(":desc", ui->edDesc->text());
    sql.bindValue(":article", ui->edArticle->text());
    sql.bindValue(":unit", ui->cmbUnit->currentKey().toString());
    sql.bindValue(":type", ui->cmbType->currentKey().toString());
    sql.bindValue(":active", ui->ckActive->isChecked());

    if (!sql.exec()) {
        qCritical() << sql.lastError();
        setErrorString(sql.lastError().text());
        QMessageBox::critical(this, "Error", sql.lastError().text());
        return false;
    }

    if (m_id == -1) {
        sql.exec("SELECT currval(pg_get_serial_sequence('items', 'it_id'))");

        if (sql.next()) {
            m_id = sql.value(0).toInt();
        } else {
            qWarning() << sql.lastError();
        }
    }

    emit saved();
    return true;
}
