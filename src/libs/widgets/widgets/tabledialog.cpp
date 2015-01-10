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
#include "tabledialog.h"
#include "editdialog.h"
#include "reportmanager.h"
#include "printbutton.h"
#include "advitemmodel.h"
#include "advtableview.h"

#include <QSortFilterProxyModel>
#include <QMessageBox>

TableDialog::TableDialog(QWidget *parent) :
    QDialog(parent),
    m_model(0),
    m_editWidget(0),
    m_editDialog(0),
    m_view(0)
{
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

}
QAbstractItemModel *TableDialog::model() const
{
    return m_model;
}

void TableDialog::setModel(AdvItemModel *model, int keyColumn, int keyRole)
{
    m_model = model;
    m_keyColumn = keyColumn;
    m_keyRole = keyRole;

    m_proxyModel->setSourceModel(m_model);

    if (m_view)
        m_view->setModel(m_proxyModel);
}

EditWidgetInterface *TableDialog::editWidget() const
{
    return m_editWidget;
}

void TableDialog::setEditWidget(EditWidgetInterface *widget)
{
    m_editWidget = widget;

    if (m_editDialog) {
        delete m_editDialog;
    }

    m_editDialog = new EditDialog(m_editWidget, this);
}

QString TableDialog::reportMenu() const
{
    return m_printMenu;
}

void TableDialog::setReportMenu(const QString &menu)
{
    m_printMenu = menu;
}

QVariant TableDialog::currentId() const
{
    if (!view() || !view()->currentIndex().isValid()) {
        return QVariant();
    }

    return m_proxyModel->index(view()->currentIndex().row(), m_keyColumn).data(m_keyRole);
}

void TableDialog::setCurrentId(QVariant id)
{
    if (!view()) {
        return;
    }

    for (int i = 0; i < m_proxyModel->rowCount(); i++) {
        if (m_proxyModel->index(i, m_keyColumn).data(m_keyRole) == id) {
            view()->setCurrentIndex(m_proxyModel->index(i, m_keyColumn));
            view()->selectRow(i);
        }
    }
}

AdvTableView *TableDialog::view() const
{
    return m_view;
}

void TableDialog::setView(AdvTableView *view)
{
    if (m_view) {
        m_view->disconnect(this);
    }
    m_view = view;

    if (m_model && !m_view->model()) {
        m_view->setModel(m_model);
    }

    m_view->setSortingEnabled(true);
    m_view->horizontalHeader()->setSectionsClickable(true);
    m_view->horizontalHeader()->setSectionsMovable(true);
    m_view->horizontalHeader()->setSortIndicatorShown(true);

    connect(m_view, SIGNAL(doubleClicked(QModelIndex)), SLOT(slotTableViewDoubleClicked(QModelIndex)));
}

void TableDialog::saveSettings(const QString &prefix)
{
    QString p = prefix.isEmpty() ? objectName() : prefix;

    if (p.isEmpty()) {
        qDebug() << "Opps, setting prefix is empty!";
        return;
    }
    p += "/";
    QSettings sett;
    sett.setValue(p + "viewState", view()->horizontalHeader()->saveState());
    sett.setValue(p + "viewGeometry", view()->saveHeaderGeometry());
    sett.setValue(p + "pos", m_pos);
    sett.setValue(p + "size", size());
}

void TableDialog::restoreSettings(const QString &prefix)
{
    QString p = prefix.isEmpty() ? objectName() : prefix;

    if (p.isEmpty()) {
        qDebug() << "Opps, setting prefix is empty!";
        return;
    }

    p += "/";
    QSettings sett;
    view()->horizontalHeader()->restoreState(sett.value(p + "viewState").toByteArray());
    view()->restoreHeaderGeometry(sett.value(p + "viewGeometry").toByteArray());
    move(sett.value(p + "pos").toPoint());
    resize(sett.value(p + "size").toSize());
}

void TableDialog::add()
{
    if (!m_editDialog || !m_model) {
        return;
    }

    if (m_editDialog->exec() == QDialog::Accepted) {
        m_model->populate();
        setCurrentId(editWidget()->id());

        view()->setFocus();
    }
}

void TableDialog::editCurrent()
{
    if (!m_editDialog || !m_model) {
        return;
    }

    if (m_editDialog->exec(currentId()) == QDialog::Accepted) {
        m_model->populate();
        view()->setFocus();

        setCurrentId(editWidget()->id());
        view()->setFocus();
    }
}

void TableDialog::deleteSelected()
{
    if (!m_model) {
        return;
    }

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

void TableDialog::slotTableViewDoubleClicked(QModelIndex index)
{
    if (index.flags() & Qt::ItemIsEditable) {
        return;
    }

    editCurrent();
}

void TableDialog::hideEvent(QHideEvent *e)
{
    m_pos = pos();
    QDialog::hideEvent(e);
}

QList<int> TableDialog::sourceRowsFromProxy(QModelIndexList indexes) const
{
    QListIterator<QModelIndex> iter(indexes);
    QList<int> rows;

    while(iter.hasNext()) {
        rows << m_proxyModel->mapToSource(iter.next()).row();
    }

    qSort(rows);
    return rows;
}
