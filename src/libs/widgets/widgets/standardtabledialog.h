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

namespace Ui {
class StandardTableDialog;
}

class QAbstractItemModel;
class EditWidgetInterface;
class EditDialog;

class StandardTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StandardTableDialog(QWidget *parent = 0);
    ~StandardTableDialog();

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *model);

    EditWidgetInterface *editWidget() const;
    void setEditWidget(EditWidgetInterface *widget);


private:
    Ui::StandardTableDialog *ui;
    QAbstractItemModel *m_model;
    EditWidgetInterface *m_editWidget;
    EditDialog *m_editDialog;
};

#endif // STANDARDTABLEDIALOG_H
