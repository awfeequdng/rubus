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
#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include "widgets_global.h"

#include <QDialog>
#include <QModelIndexList>

class QSortFilterProxyModel;
class AdvItemModel;
class EditWidgetInterface;
class EditDialog;
class AdvTableView;

class WIDGETS_EXPORT TableDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableDialog(QWidget *parent = 0);

    QAbstractItemModel *model() const;
    void setModel(AdvItemModel *model, int keyColumn = 0, int keyRole = Qt::DisplayRole);

    EditWidgetInterface *editWidget() const;
    void setEditWidget(EditWidgetInterface *widget);

    QString reportMenu() const;
    void setReportMenu(const QString &menu);

    QVariant currentId() const;
    void setCurrentId(QVariant id);

    AdvTableView *view() const;
    void setView(AdvTableView *view);


    void saveSettings(const QString &prefix = QString());
    void restoreSettings(const QString &prefix = QString());

signals:

public slots:
    void add();
    void editCurrent();
    void deleteSelected();

private slots:
    void slotTableViewDoubleClicked(QModelIndex index);


protected:
    void hideEvent(QHideEvent *e);

private:
    AdvItemModel *m_model;
    QSortFilterProxyModel *m_proxyModel;
    EditWidgetInterface *m_editWidget;
    EditDialog *m_editDialog;
    QString m_printMenu;
    AdvTableView *m_view;

    int m_keyColumn;
    int m_keyRole;
    QPoint m_pos;

    QList<int> sourceRowsFromProxy(QModelIndexList indexes) const;



};

#endif // TABLEDIALOG_H
