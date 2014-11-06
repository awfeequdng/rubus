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
#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>


#include "widgets_global.h"
#include "widgets/editwidgetinterface.h"

class WIDGETS_EXPORT EditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EditDialog(EditWidgetInterface *editWidget, QWidget *parent = 0);

    EditWidgetInterface *editWidget() const { return m_editWidget; }

signals:

public slots:
    void accept();
    int exec();
    int exec(QVariant id);

    void show();

private:

    EditWidgetInterface *m_editWidget;
    QVBoxLayout *m_centrLay;

    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void closeEvent(QCloseEvent *e);

private slots:
    void onEditWidgetSaved();

};

#endif // EDITDIALOG_H
