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
#include "usereditwidget.h"
#include "ui_usereditwidget.h"
#include "user.h"

#include <QtSql>
#include <QDebug>

UserEditWidget::UserEditWidget(QWidget *parent) :
    EditWidgetInterface(parent),
    ui(new Ui::UserEditWidget)
{
    ui->setupUi(this);

    m_contractorModel = new QSqlQueryModel(this);
    m_user = new Core::User(QString(),this);

    connect(ui->btnSave, SIGNAL(clicked()), SLOT(save()));
    connect(ui->btnCancel, SIGNAL(clicked()), SIGNAL(rejected()));
}

UserEditWidget::~UserEditWidget()
{
    delete ui;
}


QVariant UserEditWidget::id() const
{
    return m_role;
}

bool UserEditWidget::load(QVariant id)
{
    m_role = id.toString();
    populate();

    ui->edRole->setText(m_role);
    ui->edRole->setEnabled(m_role.isEmpty());

    if (!m_role.isEmpty()) {
        QSqlQuery sql;
        sql.exec(QString("SELECT up_name, up_contractor, up_params, up_gui "
                         "FROM user_params WHERE up_role = '%1'")
                 .arg(m_role));

        if (sql.lastError().isValid()) {
            setErrorString(sql.lastError().text());
            qCritical() << sql.lastError();
            return false;
        }

        if (!sql.next()) {
            setErrorString(tr("User #%1 not found!").arg(m_role));
            return false;
        }

        ui->edName->setText(sql.value(0).toString());
        ui->cmbContractor->setCurrentKey(sql.value(1).toInt());

        //m_params = sql.value(2).toString();
        //m_gui = sql.value(3).toString();
    }

    setWindowTitle(m_role.isEmpty() ? tr("New user") : tr("Edit  user #%1").arg(m_role));
    return true;
}

bool UserEditWidget::save()
{
    if (!isValid()) {
        return false;
    }

    QSqlQuery sql;
    QSqlDatabase::database().transaction();

    if (m_role.isEmpty()) {
        sql.prepare("INSERT INTO user_params (up_role, up_name, up_contractor, up_params, up_gui) "
                    "VALUES (:role, :name, :contractor, :params, :gui)");
        sql.bindValue(":role", ui->edRole->text());
    } else {
        sql.prepare("UPDATE user_params SET up_name = :name, "
                    "up_contractor = :contractor, "
                    "up_params = :params, "
                    "up_gui = :gui "
                    "WHERE up_role = :role");
        sql.bindValue(":role", m_role);
    }

    sql.bindValue(":name", ui->edName->text().simplified());
    sql.bindValue(":contractor", ui->cmbContractor->currentKey());
    //sql.bindValue(":params", m_params); //TODO get params
    //sql.bindValue(":gui", m_gui);

    if (!sql.exec()) {
        setErrorString(sql.lastError().text());
        qCritical() << sql.lastError();
        return false;
    }

    if (m_role.isEmpty()) {
        sql.exec(QString("SELECT rolname FROM pg_catalog.pg_roles "
                         "WHERE rolname = '%1'").arg(ui->edRole->text()));

        if (sql.lastError().isValid()) {
            setErrorString(sql.lastError().text());
            QSqlDatabase::database().rollback();
            return false;
        }

        if (!sql.next()) {

        }
    }


    emit saved();
    return true;
}

void UserEditWidget::populate()
{
    m_contractorModel->setQuery("SELECT co_id, co_name FROM contractors ORDER BY co_name");
    ui->cmbContractor->setModel(m_contractorModel, 0, 1);
}

bool UserEditWidget::isValid()
{
    if (ui->edRole->text().isEmpty()) {
        ui->edRole->setFocus();
        QMessageBox::critical(this, "Validation", tr("Role is empty!"));
        return false;
    }

    if (ui->edName->text().isEmpty()) {
        ui->edName->setFocus();
        QMessageBox::critical(this, "Validation", tr("Name is empty!"));
        return false;
    }

    return true;
}

