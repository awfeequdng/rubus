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
#include "usertabledialog.h"
#include "ui_usertabledialog.h"
#include "models/usermodel.h"
#include "widgets/usereditwidget.h"
#include "../../plugins/reports/reportmanager.h"
#include "widgets/editdialog.h"

#include <QMessageBox>

UserTableDialog::UserTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserTableDialog)
{
    ui->setupUi(this);

    m_model = new UserModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_editWdg = new UserEditWidget();
    m_editDialog = new EditDialog(m_editWdg, this);
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterKeyColumn(UserModel::NameCol);


    ui->tableView->horizontalHeader()->setDefaultSectionSize(UserModel::RoleCol, 180);
    ui->tableView->setModel(m_proxyModel, UserModel::RoleCol, Qt::DisplayRole);

    connect(ui->btnAdd, SIGNAL(clicked()), SLOT(add()));
    connect(ui->btnEdit, SIGNAL(clicked()), SLOT(editCurrent()));
    connect(ui->btnDelete, SIGNAL(clicked()), SLOT(deleteSelected()));
    connect(ui->btnPrint, SIGNAL(print(Report&)), SLOT(slotPrint(Report&)));
    connect(ui->edFind, SIGNAL(textEdited(QString)), m_proxyModel, SLOT(setFilterRegExp(QString)));
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(viewDoubleClicked(QModelIndex)));

    restoreSettings();
}

UserTableDialog::~UserTableDialog()
{
    saveSettings();
    delete ui;
}

AdvTableView *UserTableDialog::view() const
{
    return ui->tableView;
}

void UserTableDialog::saveSettings(const QString &prefix)
{
    QString p = prefix.isEmpty() ? objectName() : prefix;

    if (p.isEmpty()) {
        qDebug() << "Opps, setting prefix is empty!";
        return;
    }

    QSettings sett;
    sett.setValue(p + "/viewState", view()->horizontalHeader()->saveState());
    sett.setValue(p + "/viewGeometry", view()->saveHeaderGeometry());
    sett.setValue(p + "/pos", m_pos);
    sett.setValue(p + "/size", size());
}


void UserTableDialog::restoreSettings(const QString &prefix)
{
    QString p = prefix.isEmpty() ? objectName() : prefix;

    if (p.isEmpty()) {
        qDebug() << "Opps, setting prefix is empty!";
        return;
    }

    QSettings sett;
    view()->horizontalHeader()->restoreState(sett.value(p + "/viewState").toByteArray());
    view()->restoreHeaderGeometry(sett.value(p + "/viewGeometry").toByteArray());
    move(sett.value(p + "/pos").toPoint());
    resize(sett.value(p + "/size").toSize());
}

void UserTableDialog::add()
{
    if (m_editDialog->exec() == QDialog::Accepted) {
        m_model->populate();
        view()->setCurrentId(m_editWdg->id());
        view()->setFocus();
    }
}

void UserTableDialog::editCurrent()
{
    if (m_editDialog->exec(view()->currentId()) == QDialog::Accepted) {
        m_model->populate();
        view()->setFocus();

        view()->setCurrentId(m_editWdg->id());
        view()->setFocus();
    }
}

void UserTableDialog::deleteSelected()
{
    QModelIndexList rows = view()->selectionModel()->selectedRows();

    if (rows.isEmpty()) {
        return;
    }

    if (QMessageBox::warning(this,
                             tr("Delete"),
                             tr("Are you sure you want to remove selected items(%1)").arg(rows.count()),
                             QMessageBox::Cancel | QMessageBox::Ok)
            == QMessageBox::Cancel) {
        return;
    }

    QListIterator<int> iter(sourceRowsFromProxy(rows));
    iter.toBack();
    while(iter.hasPrevious()) {
        if (!m_model->removeRow(iter.previous())) {
            m_model->revert();
            QMessageBox::critical(this, tr("Delete"), m_model->errorString());
            return;
        }
    }

    if (!m_model->submit()) {
        QMessageBox::critical(this, tr("Delete"), m_model->errorString());
    }
}

void UserTableDialog::slotPrint(Report &r)
{
    r.appendModel(m_model);
    ReportManager::showReport(r);
}

void UserTableDialog::viewDoubleClicked(QModelIndex index)
{
    if (index.flags() & Qt::ItemIsEditable) {
        return;
    }

    editCurrent();
}

void UserTableDialog::showEvent(QShowEvent *e)
{
    if (!m_model->populate()) {
        QMessageBox::critical(this, tr("Error"), m_model->errorString());
    }
    QDialog::showEvent(e);
}

void UserTableDialog::hideEvent(QHideEvent *e)
{
    m_pos = pos();
    QDialog::hideEvent(e);
}

QList<int> UserTableDialog::sourceRowsFromProxy(QModelIndexList indexes) const
{
    QListIterator<QModelIndex> iter(indexes);
    QList<int> rows;

    while(iter.hasNext()) {
        rows << m_proxyModel->mapToSource(iter.next()).row();
    }

    qSort(rows);
    return rows;
}

