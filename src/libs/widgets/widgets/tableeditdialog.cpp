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
#include "tableeditdialog.h"
//#include "advtableview.h"

#include <QPushButton>
#include <QLayout>

#define MIN_HEIGHT_BTN 30

TableEditDialog::TableEditDialog(TableEditInterface *tableInterface, QWidget *parent) :
    QDialog(parent),
    m_tableWidget(tableInterface)
{
    setObjectName("TableEditDialog");
    setFont(QFont("tahoma", 10));
    m_btnAccept = new QPushButton(tr("Accept"), this);
    m_btnCancel = new QPushButton(tr("Cancel"), this);

    m_btnAccept->setMinimumHeight(MIN_HEIGHT_BTN);
    m_btnCancel->setMinimumHeight(MIN_HEIGHT_BTN);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_tableWidget);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(m_btnAccept);
    btnLayout->addWidget(m_btnCancel);

    layout->addLayout(btnLayout);
    setLayout(layout);

    layout->setMargin(6);

    setWindowTitle(m_tableWidget->windowTitle());

    setFocusProxy(tableInterface);
}

TableEditDialog::~TableEditDialog()
{
    QSettings sett;
    sett.setValue(m_tableWidget->settingPrefix() + "/geometry", saveGeometry());
}


int TableEditDialog::exec()
{
    setVisibleButtons(true);

    connect(m_btnAccept,SIGNAL(clicked()),SLOT(accept()));
    connect(m_btnCancel,SIGNAL(clicked()),SLOT(reject()));
    //connect(m_tableWidget->view(),SIGNAL(doubleClicked(QModelIndex)),SLOT(accept()));

    m_tableWidget->populate();

    return QDialog::exec();
}

int TableEditDialog::exec(QVariant selectId)
{
    setVisibleButtons(true);

    connect(m_btnAccept,SIGNAL(clicked()),SLOT(accept()));
    connect(m_btnCancel,SIGNAL(clicked()),SLOT(reject()));
    //connect(m_tableWidget->view(),SIGNAL(doubleClicked(QModelIndex)),SLOT(accept()));

    m_tableWidget->populate();
    m_tableWidget->setCurrentPrimaryKey(selectId);

    return QDialog::exec();
}

void TableEditDialog::show()
{
    setVisibleButtons(false);

    disconnect(m_btnAccept);
    disconnect(m_btnCancel);
    //disconnect(m_tableWidget->view(),0,this,0);

    m_tableWidget->populate();

    QDialog::show();
}

void TableEditDialog::setVisibleButtons(bool visible)
{
    m_btnAccept->setVisible(visible);
    m_btnCancel->setVisible(visible);
}

void TableEditDialog::showEvent(QShowEvent *e)
{
    QSettings sett;
    restoreGeometry(sett.value(m_tableWidget->settingPrefix() + "/geometry").toByteArray());

    QDialog::showEvent(e);
    m_tableWidget->setFocus();
}

void TableEditDialog::hideEvent(QHideEvent *e)
{
    QSettings sett;
    sett.setValue(m_tableWidget->settingPrefix() + "/geometry", saveGeometry());

    QDialog::hideEvent(e);
}
