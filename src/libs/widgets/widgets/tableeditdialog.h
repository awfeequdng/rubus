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
#ifndef TABLEEDITDIALOG_H
#define TABLEEDITDIALOG_H

#include <QDialog>

#include "widgets_global.h"
#include "tableeditinterface.h"

class WIDGETS_EXPORT TableEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableEditDialog(TableEditInterface *tableInterface, QWidget *parent = 0);
    ~TableEditDialog();

    TableEditInterface *tableWidget() const { return m_tableWidget; }


signals:

public slots:
    int exec();
    int exec(QVariant selectId);
    void show();

private:
    QPushButton *m_btnAccept;
    QPushButton *m_btnCancel;

    TableEditInterface *m_tableWidget;
    void setVisibleButtons(bool visible);

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

};

#endif // TABLEEDITDIALOG_H
