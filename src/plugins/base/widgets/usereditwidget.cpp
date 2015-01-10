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
#include "qjson/qjsonarray.h"
#include "qjson/qjsonobject.h"

#include <QtSql>
#include <QDebug>
#include <QMessageBox>

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
    m_user->setRoleName(m_role);
    ui->edPwd->clear();
    ui->edPwdAgain->clear();

    if (!m_user->load()) {
        qCritical() << m_user->errorString();
        return false;
    }

    ui->edName->setText(m_user->name());
    ui->cmbContractor->setCurrentKey(m_user->contractorId());
    ui->edGui->setText(m_user->gui());
    QJsonDocument doc(m_user->parameters());
    ui->edParams->setText(QString(doc.toJson()));

    setWindowTitle(m_role.isEmpty() ? tr("New user") : tr("Edit  user #%1").arg(m_role));
    return true;
}

bool UserEditWidget::save()
{
    if (!isValid()) {
        return false;
    }

    QSqlDatabase::database().transaction();

    m_user->setRoleName(ui->edRole->text());
    m_user->setName(ui->edName->text());
    m_user->setContractor(ui->cmbContractor->currentKey().toInt());
    m_user->setGui(ui->edGui->toPlainText());
    m_user->setParameters(ui->edParams->toPlainText());

    if (!m_user->save()) {
        QSqlDatabase::database().rollback();
        qCritical() << m_user->errorString();
        QMessageBox::critical(this, tr("Save"), m_user->errorString());
        return false;
    }

    if (!ui->edPwd->text().isEmpty()) {
        if (!m_user->changePassword(ui->edPwd->text())) {
            QSqlDatabase::database().rollback();
            qCritical() << m_user->errorString();
            QMessageBox::critical(this, tr("Save"), m_user->errorString());
            return false;
        }
    }

    QSqlDatabase::database().commit();
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

    if (!ui->edPwd->text().isEmpty() && ui->edPwd->text() != ui->edPwdAgain->text()) {
        ui->edPwd->setFocus();
        QMessageBox::critical(this, tr("Validation"), tr("Password and password again incorrect"));
        return false;
    }

    return true;
}

