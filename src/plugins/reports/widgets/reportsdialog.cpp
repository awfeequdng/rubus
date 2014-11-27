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
#include "reportsdialog.h"
#include "ui_reportsdialog.h"
#include "widgets/advtableview.h"
#include "widgets/advcombobox.h"
#include "widgets/advlineedit.h"
#include "widgets/reporteditwidget.h"
#include "models/reportmodel.h"
#include "reportmanager.h"
#include "report.h"

#include <QSettings>
#include <QSortFilterProxyModel>

ReportsDialog::ReportsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportsDialog)
{
    ui->setupUi(this);

    m_model = new ReportModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);

    ui->reportView->horizontalHeader()->setDefaultSectionSize(ReportModel::IdCol, 80);
    ui->reportView->horizontalHeader()->setDefaultSectionSize(ReportModel::TypeCol, 140);
    ui->reportView->setModel(m_proxyModel, ReportModel::IdCol);


    connect(ui->btnAdd, SIGNAL(clicked()), SLOT(add()));
    connect(ui->btnEdit, SIGNAL(clicked()), SLOT(editCurrent()));
    connect(ui->btnDelete, SIGNAL(clicked()), SLOT(removeSelected()));
    connect(ui->btnPreview, SIGNAL(clicked()), SLOT(previewCurrent()));
    connect(ui->wdgEdit, SIGNAL(closed()), SLOT(slotToTable()));
    connect(ui->wdgEdit, SIGNAL(saved()), SLOT(slotOnSaved()));
    connect(ui->reportView, SIGNAL(doubleClicked(QModelIndex)), SLOT(editCurrent()));


    slotToTable();
}

ReportsDialog::~ReportsDialog()
{
    delete ui;
}

void ReportsDialog::populate()
{
    m_model->populate();
    ui->reportView->sortByColumn(ui->reportView->horizontalHeader()->sortIndicatorSection(),
                                 ui->reportView->horizontalHeader()->sortIndicatorOrder());
}

int ReportsDialog::currentId() const
{
    return ui->reportView->currentId().toInt();
}

void ReportsDialog::setCurrentId(int id)
{
    ui->reportView->setCurrentId(id);
}

void ReportsDialog::showEvent(QShowEvent *event)
{
    populate();

    QSettings sett;
    restoreGeometry(sett.value("ReportsDialog/geometry").toByteArray());
    //ui->reportView->loadColumnSettings();

    QDialog::showEvent(event);
}

void ReportsDialog::hideEvent(QHideEvent *event)
{
    slotToTable();

    QSettings sett;
    sett.setValue("ReportsDialog/geometry", saveGeometry());
    //ui->reportView->saveColumnSettings();

    QDialog::hideEvent(event);
}

void ReportsDialog::add()
{
    if (ui->wdgEdit->load(-1)) {
        setWindowTitle(tr("Add report *"));
        ui->stackedWidget->setCurrentWidget(ui->wdgEdit);
    }
}

void ReportsDialog::editCurrent()
{
    if (!ui->reportView->currentIndex().isValid()) {
        return;
    }

    if (ui->wdgEdit->load(currentId())) {
        setWindowTitle(tr("Edit report #%1").arg(currentId()));
        ui->stackedWidget->setCurrentWidget(ui->wdgEdit);
    }


}

void ReportsDialog::removeSelected()
{

}

void ReportsDialog::previewCurrent()
{
    Report rep = ReportManager::loadReport(currentId());

    ReportManager::instance()->showReport(rep);
}

void ReportsDialog::slotToTable()
{
    setWindowTitle(tr("Reports"));
    ui->stackedWidget->setCurrentWidget(ui->pageTable);
}

void ReportsDialog::slotOnSaved()
{
    populate();

    ui->reportView->setCurrentId(ui->wdgEdit->id());
    slotToTable();

}
