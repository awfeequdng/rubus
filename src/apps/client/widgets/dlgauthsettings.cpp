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

#include "dlgauthsettings.h"
#include "ui_dlgauthsettings.h"

DlgAuthSettings::DlgAuthSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAuthSettings)
{
    ui->setupUi(this);
}

DlgAuthSettings::~DlgAuthSettings()
{
    delete ui;
}

QString DlgAuthSettings::hostname() const
{
    return ui->edHost->text();
}

QString DlgAuthSettings::database() const
{
    return ui->edDatabase->text();
}

int DlgAuthSettings::port() const
{
    return ui->edPort->value();
}

void DlgAuthSettings::setHostname(const QString host)
{
    ui->edHost->setText(host);
}

void DlgAuthSettings::setDatabase(const QString database)
{
    ui->edDatabase->setText(database);
}

void DlgAuthSettings::setPort(const int port)
{
    ui->edPort->setValue(port);
}
