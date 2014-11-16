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
#ifndef CONTRACTORTABLEDIALOG_H
#define CONTRACTORTABLEDIALOG_H

#include "widgets/tabledialog.h"
#include "report.h"

namespace Ui {
class ContractorTableDialog;
}

class ContractorModel;
class ContractorEditWidget;

class ContractorTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContractorTableDialog(QWidget *parent = 0);
    ~ContractorTableDialog();

    AdvTableView *view() const;

    void saveSettings(const QString &prefix = QString());
    void restoreSettings(const QString &prefix = QString());

public slots:
    void add();
    void editCurrent();
    void deleteSelected();
    void slotPrint(Report &r);

private slots:
    void viewDoubleClicked(QModelIndex index);

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private:
    Ui::ContractorTableDialog *ui;
    ContractorModel *m_model;
    QSortFilterProxyModel *m_proxyModel;
    ContractorEditWidget *m_editWdg;
    EditDialog *m_editDialog;
    QPoint m_pos;

    QList<int> sourceRowsFromProxy(QModelIndexList indexes) const;

};

#endif // CONTRACTORTABLEDIALOG_H
