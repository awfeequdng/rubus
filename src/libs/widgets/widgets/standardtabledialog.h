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
#ifndef STANDARDTABLEDIALOG_H
#define STANDARDTABLEDIALOG_H

#include <QDialog>
#include <QModelIndexList>
#include "report.h"
#include "widgets_global.h"

namespace Ui {
class StandardTableDialog;
}

class QSortFilterProxyModel;
class AdvItemModel;
class EditWidgetInterface;
class EditDialog;

class WIDGETS_EXPORT StandardTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StandardTableDialog(QWidget *parent = 0);
    ~StandardTableDialog();

    QAbstractItemModel *model() const;
    void setModel(AdvItemModel *model, int keyColumn = 0, int keyRole = Qt::DisplayRole);

    EditWidgetInterface *editWidget() const;
    void setEditWidget(EditWidgetInterface *widget);

    QString reportMenu() const;
    void setReportMenu(const QString &menu);

    QVariant currentId() const;
    void setCurrentId(QVariant id);

public slots:
    void add();
    void editCurrent();
    void deleteSelected();
    void slotPrint(Report & report);

private:
    Ui::StandardTableDialog *ui;
    AdvItemModel *m_model;
    QSortFilterProxyModel *m_proxyModel;
    EditWidgetInterface *m_editWidget;
    EditDialog *m_editDialog;
    QString m_printMenu;

    int m_keyColumn;
    int m_keyRole;

    QList<int> sourceRowsFromProxy(QModelIndexList indexes) const;
};

#endif // STANDARDTABLEDIALOG_H
