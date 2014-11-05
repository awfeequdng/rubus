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

#include "dlgauthorise.h"
#include "ui_dlgauthorise.h"
#include "coreconstants.h"

#include <QSettings>
#include <QtSql>

using namespace Core;

DlgAuthorise::DlgAuthorise(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAuthorise),
    m_core(ICore::instance())
{
    ui->setupUi(this);
    setWindowTitle(QString("Rubus %1").arg(ICore::version()));
    connect(ui->btnSettings,SIGNAL(clicked()),SLOT(showAuthSettings()));

    setWindowFlags(Qt::Dialog |
                   Qt::WindowTitleHint |
                   Qt::WindowCloseButtonHint |
                   Qt::WindowStaysOnTopHint |
                   Qt::MSWindowsFixedSizeDialogHint);
}

DlgAuthorise::~DlgAuthorise()
{    
    delete ui;
}

void DlgAuthorise::setUserName(const QString &name)
{
    ui->edUsername->setText(name);
}

void DlgAuthorise::setPassword(const QString &pwd)
{
    ui->edPassword->setText(pwd);
}

void DlgAuthorise::showEvent ( QShowEvent * event )
{

    event->accept();

    if (ui->edUsername->text().isEmpty())
        ui->edUsername->setFocus();
    else {
        if (ui->edPassword->text().isEmpty())
            ui->edPassword->setFocus();
        else {
            ui->edPassword->selectAll();
            ui->edPassword->setFocus();
        }
    }
}

int DlgAuthorise::exec()
{
    loadSettings();

    if (isRemember() && authorise()) {
        return QDialog::Accepted;
    }    

    return QDialog::exec();
}

void DlgAuthorise::showAuthSettings()
{
    DlgAuthSettings *dlg = new DlgAuthSettings(this);
    dlg->setHostname(m_core->databaseHost());
    dlg->setDatabase(m_core->databaseName());
    dlg->setPort(m_core->databasePort());

    if (dlg->exec() == QDialog::Accepted) {
        m_core->setDatabaseHost(dlg->hostname());
        m_core->setDatabaseName(dlg->database());
        m_core->setDatabasePort(dlg->port());
    }

    dlg->deleteLater();
}

bool DlgAuthorise::authorise()
{
    if (!m_core->login(ui->edUsername->text(),
                       ui->edPassword->text())) {

        QMessageBox::critical(this,tr("Error"),m_core->lastError());
        ui->edPassword->setFocus();
        return false;
    } else {
        return true;
    }
}

void DlgAuthorise::accept()
{
    if (authorise()) {
        saveSettings();
        QDialog::accept();
    }

}

void DlgAuthorise::loadSettings()
{
    QSettings set;
    ui->edUsername->setText( set.value("lastUser",QString()).toString() );
    ui->ckRemember->setChecked( set.value("remember", false).toBool() );
    ui->btnSettings->setVisible(m_core->canChangeDatabaseSettings());

    if (isRemember()) {
        ui->edPassword->setText( Cryptor::decode(set.value("passwd").toString()) );
    }
}

void DlgAuthorise::saveSettings()
{
    QSettings set;
    set.setValue("lastUser", ui->edUsername->text());
    set.setValue("remember", ui->ckRemember->isChecked());

    if (isRemember()) {
        set.setValue("passwd", Cryptor::encode(ui->edPassword->text()) );
    }
}

QString DlgAuthorise::username()
{
    return ui->edUsername->text();
}

bool DlgAuthorise::isRemember()
{
    return ui->ckRemember->isChecked();
}
