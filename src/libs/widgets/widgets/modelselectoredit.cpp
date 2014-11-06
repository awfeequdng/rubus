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
#include "modelselectoredit.h"

#include "advlineedit.h"
#include "advcombobox.h"

#include <QLayout>
#include <QLabel>
#include <QAbstractItemModel>
#include <QDebug>
#include <QAction>

ModelSelectorEdit::ModelSelectorEdit(QWidget *parent) :
    QWidget(parent),
    m_model(0),
    m_id(-1)
{
    m_edNumber = new AdvLineEdit(this);
    m_labName = new QLabel(this);
    m_comboBox = new AdvComboBox(this);
    m_comboBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    m_labName->setVisible(false);

    m_edNumber->setMaximumWidth(80);
    m_edNumber->setMinimumWidth(80);

    QHBoxLayout *centrLay = new QHBoxLayout(this);
    centrLay->addWidget(m_edNumber);
    centrLay->addWidget(m_labName);
    centrLay->addWidget(m_comboBox);
    centrLay->addStretch();
    centrLay->setStretchFactor(m_comboBox,10);

    setFocusProxy(m_edNumber);
    m_comboBox->setFocusProxy(m_edNumber);
    centrLay->setMargin(0);

    QAction *acChoose = new QAction(parent);
    acChoose->setShortcutContext(Qt::WidgetShortcut);
    acChoose->setShortcut(Qt::Key_F3);
    connect(acChoose, SIGNAL(triggered()), SLOT(activateCombo()));

    m_edNumber->addAction(acChoose);

    connect(m_edNumber, SIGNAL(editingFinished()), SLOT(numberEditFinished()));
    connect(m_edNumber, SIGNAL(returnPressed()), SIGNAL(editReturnPressed()));
    connect(m_comboBox, SIGNAL(keyChanged()), SLOT(onComboBoxChanged()));
}

void ModelSelectorEdit::setVisibleComboBox(bool visible)
{
    m_labName->setVisible(!visible);
    m_comboBox->setVisible(visible);
}

bool ModelSelectorEdit::isVisibleComboBox() const
{
    return m_comboBox->isVisible();
}

int ModelSelectorEdit::currentId() const
{
    return  m_edNumber->text().toInt();
}

void ModelSelectorEdit::setCurrentId(int id)
{
    m_edNumber->setText(QString::number(id));
    m_labName->setText(displayData(id));
    m_id = id;

    m_comboBox->setCurrentKey(id);
    emit currentIdChanged(currentId());
}

void ModelSelectorEdit::setModel(QAbstractItemModel *model, int idCol, int displayCol)
{
    m_model = model;
    m_idCol = idCol;
    m_displayCol = displayCol;


    m_comboBox->setModel(model, idCol, displayCol);
}

bool ModelSelectorEdit::isValid() const
{
    return contains(currentId());
}

void ModelSelectorEdit::numberEditFinished()
{
    if (contains(currentId())) {
        emit editFinished();
    } else {
        m_edNumber->selectAll();
    }

    m_labName->setText(displayData(currentId()));

    if (m_comboBox->isVisible()) {
        m_comboBox->blockSignals(true);
        m_comboBox->setCurrentKey(contains(currentId()) ? currentId() : -1);
        m_comboBox->blockSignals(false);
    }

    emit currentIdChanged(currentId());
}

void ModelSelectorEdit::onComboBoxChanged()
{
    setCurrentId(m_comboBox->currentKey().toInt());
}

void ModelSelectorEdit::activateCombo()
{
    m_comboBox->setFocus();
    m_comboBox->showPopup();
}

QString ModelSelectorEdit::displayData(QVariant id) const
{
    QString text = tr("Not found");
    for (int i = 0; i < m_model->rowCount(); i++) {
        if (m_model->index(i, m_idCol).data(Qt::DisplayRole) == id) {
            text = m_model->index(i, m_displayCol).data(Qt::DisplayRole).toString();
            break;
        }
    }

    return text;
}

bool ModelSelectorEdit::contains(QVariant id) const
{
    if (!m_model) {
        return false;
    }

    for (int i = 0; i < m_model->rowCount(); i++) {
        if (m_model->index(i, m_idCol).data(Qt::DisplayRole) == id) {
            return true;
        }
    }

    return false;
}
