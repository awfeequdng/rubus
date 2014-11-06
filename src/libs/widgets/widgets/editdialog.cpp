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

#define MIN_HEIGHT_BTN 30

EditDialog::EditDialog(EditWidgetInterface *editWidget, QWidget *parent) :
    QDialog(parent),
    m_editWidget(editWidget),
    m_closeAfterSave(true),
    m_saved(false)
{
    setFont(QFont("Tahoma",10));
    setWindowTitle(editWidget->windowTitle());

    m_btnSave = new QPushButton(tr("Save"),this);
    m_btnCancel = new QPushButton(tr("Cancel"),this);
    m_btnSave->setMinimumHeight(MIN_HEIGHT_BTN);
    m_btnCancel->setMinimumHeight(MIN_HEIGHT_BTN);

    QHBoxLayout *objectActionsLay = new QHBoxLayout();
    objectActionsLay->addStretch();
    objectActionsLay->addWidget(m_btnSave);
    objectActionsLay->addWidget(m_btnCancel);

    m_btnSave->setAutoDefault(false);
    m_btnCancel->setAutoDefault(false);
    m_btnSave->setDefault(false);
    m_btnCancel->setDefault(false);

    QVBoxLayout *centrLay = new QVBoxLayout(this);
    centrLay->addWidget(editWidget);

    QShortcut *scSave = new QShortcut(this);
    scSave->setKey(QKeySequence::Save);
    connect(scSave, SIGNAL(activated()), m_editWidget, SLOT(save()));

    QShortcut *scAccept = new QShortcut(this);
    scAccept->setKey(Qt::CTRL + Qt::Key_Return);
    connect(scAccept, SIGNAL(activated()), SLOT(accept()));


    centrLay->addLayout(objectActionsLay);
    centrLay->setMargin(4);
    setLayout(centrLay);

    setFocusProxy(editWidget);


    connect(m_btnCancel,SIGNAL(clicked()),SLOT(reject()));
    connect(editWidget,SIGNAL(windowTitleChanged(QString)),
            SLOT(setWindowTitle(QString)));
    connect(editWidget, SIGNAL(saved()), SLOT(onEditWidgetSaved()));
}

void EditDialog::setCloseAfterSave(bool close)
{
    m_closeAfterSave = close;
}

bool EditDialog::isCloseAfterSave() const
{
    return m_closeAfterSave;
}

void EditDialog::setDefaultButton(bool enabled)
{
    m_btnSave->setDefault(enabled);
    m_btnCancel->setDefault(enabled);
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
    m_btnSave->disconnect();
    connect(m_btnSave,SIGNAL(clicked()),SLOT(accept()));

    if (!editWidget()) {
        qDebug() << "ObjectWidget is not set";
        return QDialog::Rejected;
    }

    if (!editWidget()->load(id)) {
        qDebug() << "ObjectWidget is not loaded";
        QMessageBox::critical(this,tr("Object load"),editWidget()->lastError());
        return QDialog::Rejected;
    }

    return QDialog::exec();
}

void EditDialog::show()
{
    m_btnSave->disconnect();
    connect(m_btnSave,SIGNAL(clicked()), m_editWidget, SLOT(save()));

    QDialog::show();
}

void EditDialog::showEvent(QShowEvent *e)
{
    QSettings sett;

    restoreGeometry(sett.value(editWidget()->settingPrefix() + "/geometry").toByteArray());

    QDialog::showEvent(e);

    editWidget()->setFocus();
}

void EditDialog::hideEvent(QHideEvent *e)
{
    QDialog::hideEvent(e);

    QSettings sett;

    sett.setValue(editWidget()->settingPrefix() + "/geometry",saveGeometry());
    //sett.setValue(editWidget()->settingPrefix() + "/pos", geometry().);
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
                QMessageBox::critical(this, tr("Save"), editWidget()->lastError());
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
    if (m_closeAfterSave) {
        QDialog::accept();
    }
}
