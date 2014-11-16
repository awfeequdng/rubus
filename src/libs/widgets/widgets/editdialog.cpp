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
#include "editdialog.h"

#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>
#include <QPushButton>
#include <QLayout>
#include <QDebug>

EditDialog::EditDialog(EditWidgetInterface *editWidget, QWidget *parent) :
    QDialog(parent),
    m_editWidget(editWidget)
{
    setWindowTitle(editWidget->windowTitle());

    m_centrLay = new QVBoxLayout(this);
    m_centrLay->addWidget(editWidget);
    m_centrLay->setMargin(4);

    setFocusProxy(editWidget);

    connect(editWidget, SIGNAL(saved()), SLOT(onEditWidgetSaved()));
    connect(editWidget, SIGNAL(windowTitleChanged(QString)), SLOT(setWindowTitle(QString)));
    connect(editWidget, SIGNAL(rejected()), SLOT(reject()));
}

void EditDialog::accept()
{
    if (editWidget()->save()) {
        QDialog::accept();
    }
}

int EditDialog::exec()
{
    return exec(QVariant());
}

int EditDialog::exec(QVariant id)
{

    if (!editWidget()) {
        qDebug() << "ObjectWidget is not set";
        return QDialog::Rejected;
    }

    if (!editWidget()->load(id)) {
        qDebug() << "ObjectWidget is not loaded";
        QMessageBox::critical(this,tr("Object load"),editWidget()->errorString());
        return QDialog::Rejected;
    }

    return QDialog::exec();
}

void EditDialog::show()
{
    QDialog::show();
}

void EditDialog::showEvent(QShowEvent *e)
{
    QSettings sett;
    resize(sett.value(editWidget()->settingPrefix() + "/size").toSize());
    QDialog::showEvent(e);

    editWidget()->setFocus();
}

void EditDialog::hideEvent(QHideEvent *e)
{
    QDialog::hideEvent(e);

    QSettings sett;
    sett.setValue(editWidget()->settingPrefix() + "/size",editWidget()->size());
}

void EditDialog::closeEvent(QCloseEvent *e)
{
    if (editWidget()->isDataChanged()) {
        int result = QMessageBox::question(this,tr("Close"),tr("Do you want to save the changes before closing?"),
                                           QMessageBox::Yes |
                                           QMessageBox::No |
                                           QMessageBox::Cancel);


        if (result == QMessageBox::Cancel) {
            e->ignore();
            return;
        }
        if (result == QMessageBox::No) {
            e->accept();
            return;
        }

        if (result == QMessageBox::Yes) {
            if (!editWidget()->save()) {
                QMessageBox::critical(this, tr("Save"), editWidget()->errorString());
                e->ignore();
            } else {
                e->accept();
            }
        }
    } else {
        e->accept();
    }
}

void EditDialog::onEditWidgetSaved()
{
    QDialog::accept();
}
