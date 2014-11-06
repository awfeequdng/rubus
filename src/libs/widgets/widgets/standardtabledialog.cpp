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
#include "reportmanager.h"
#include "printbutton.h"
#include "advitemmodel.h"

#include <QSortFilterProxyModel>

StandardTableDialog::StandardTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StandardTableDialog),
    m_model(0),
    m_editWidget(0),
    m_editDialog(0)
{
    ui->setupUi(this);

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    connect(ui->btnAdd, SIGNAL(clicked()), SLOT(add()));
    connect(ui->btnEdit, SIGNAL(clicked()), SLOT(editCurrent()));
    connect(ui->btnDelete, SIGNAL(clicked()), SLOT(deleteSelected()));
    connect(ui->btnPrint, SIGNAL(print(Report&)), SLOT(slotPrint(Report&)));
}

StandardTableDialog::~StandardTableDialog()
{
    delete ui;
}

QAbstractItemModel *StandardTableDialog::model() const
{
    return m_model;
}

void StandardTableDialog::setModel(AdvItemModel *model, int keyColumn, int keyRole)
{
    m_model = model;
    m_keyColumn = keyColumn;
    m_keyRole = keyRole;

    m_proxyModel->setSourceModel(m_model);
    ui->tableView->setModel(m_proxyModel);
}

EditWidgetInterface *StandardTableDialog::editWidget() const
{
    return m_editWidget;
}

void StandardTableDialog::setEditWidget(EditWidgetInterface *widget)
{
    m_editWidget = widget;

    if (m_editDialog) {
        delete m_editDialog;
    }

    m_editDialog = new EditDialog(m_editWidget, this);
}

QString StandardTableDialog::reportMenu() const
{
    return m_printMenu;
}

void StandardTableDialog::setReportMenu(const QString &menu)
{
    m_printMenu = menu;
}

QVariant StandardTableDialog::currentId() const
{
    if (!m_model || !ui->tableView->currentIndex().isValid()) {
        return QVariant();
    }

    return m_proxyModel->index(ui->tableView->currentIndex().row(), m_keyColumn).data(m_keyRole);
}

void StandardTableDialog::setCurrentId(QVariant id)
{
    if (!m_model) {
        return;
    }

    for (int i = 0; i < m_proxyModel->rowCount(); i++) {
        if (m_proxyModel->index(i, m_keyColumn).data(m_keyRole) == id) {
            ui->tableView->setCurrentIndex(m_proxyModel->index(i, m_keyColumn));
            ui->tableView->selectRow(i);
        }
    }
}

void StandardTableDialog::add()
{
    if (!m_editDialog && !m_model) {
        return;
    }

    if (m_editDialog->exec() == QDialog::Accepted) {
        m_model->populate();
    }
}

void StandardTableDialog::editCurrent()
{
    if (!m_editDialog && !m_model) {
        return;
    }

    if (m_editDialog->exec(currentId()) == QDialog::Accepted) {
        m_model->populate();
    }
}

void StandardTableDialog::deleteSelected()
{
    if (!m_model) {
        return;
    }
}

void StandardTableDialog::slotPrint(Report &report)
{
    Core::ReportManager::showReport(report);
}
