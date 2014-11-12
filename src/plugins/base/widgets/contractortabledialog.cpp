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
#include "contractortabledialog.h"
#include "ui_contractortabledialog.h"
#include "models/contractormodel.h"
#include "constants.h"
#include "reportmanager.h"

#include <QMessageBox>

ContractorTableDialog::ContractorTableDialog(QWidget *parent) :
    TableDialog(parent),
    ui(new Ui::ContractorTableDialog)
{
    ui->setupUi(this);

    m_model = new ContractorModel(this);

    ui->tableView->horizontalHeader()->setDefaultSectionSize(ContractorModel::IdCol, 50);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(ContractorModel::TypeCol, 150);
    setModel(m_model, ContractorModel::IdCol, Qt::DisplayRole);
    setView(ui->tableView);

    connect(ui->btnAdd, SIGNAL(clicked()), SLOT(add()));
    connect(ui->btnEdit, SIGNAL(clicked()), SLOT(editCurrent()));
    connect(ui->btnDelete, SIGNAL(clicked()), SLOT(deleteSelected()));
    connect(ui->btnPrint, SIGNAL(print(Report&)), SLOT(slotPrint(Report&)));



    restoreSettings();
}

ContractorTableDialog::~ContractorTableDialog()
{
    saveSettings();
    delete ui;
}

void ContractorTableDialog::slotPrint(Report &r)
{
    r.appendModel(m_model);
    Core::ReportManager::showReport(r);
}

void ContractorTableDialog::showEvent(QShowEvent *e)
{
    if (!m_model->populate()) {
        QMessageBox::critical(this, tr("Error"), m_model->errorString());
    }

    ui->btnPrint->populateMenu(Constants::A_CONTRACTORS);

    TableDialog::showEvent(e);
}
