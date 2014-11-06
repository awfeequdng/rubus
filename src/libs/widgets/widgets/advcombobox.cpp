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
#include "advcombobox.h"
#include <QDebug>
#include <QLineEdit>
#include <QAbstractProxyModel>
#include <QCompleter>
#include "advlineedit.h"

AdvComboBox::AdvComboBox(QWidget *parent) :
    QComboBox(parent)
{
    m_keyColRole = Qt::DisplayRole;
    m_displayColRole = Qt::DisplayRole;
    m_currentKey = -1;    

    connect(this, SIGNAL(currentIndexChanged(int)), SLOT(onIndexChanged()));
    connect(this, SIGNAL(editTextChanged(QString)), SLOT(onCurrentTextChanged()));
}

void AdvComboBox::setModel(QAbstractItemModel *model, int keyCol, int displayCol)
{
    if (model != QComboBox::model()) {
        QComboBox::setModel(model);
    }
    QComboBox::setModelColumn(displayCol);

    m_keyCol = keyCol;
    m_displayCol = displayCol;

}

void AdvComboBox::setCurrentKey(QVariant key)
{
    if (!model()) {
        return;
    }

    if (!key.isValid() || key.toInt() == -1) {
        setCurrentIndex(-1);
        return;
    }

    for (int i = 0; i < model()->rowCount(); i++) {
        if (model()->index(i, m_keyCol).data(m_keyColRole) == key) {
            setCurrentIndex(i);

            if (m_currentKey != key) {
                emit keyChanged();
                emit keyChanged(key);
            }

            m_currentKey = key;

            break;
        }
    }
}

QVariant AdvComboBox::currentKey() const
{
    if (!model() || currentIndex() == -1) {
        return -1;
    }

    return model()->index(currentIndex(),m_keyCol).data(m_displayColRole);
}

void AdvComboBox::setCompleter(QCompleter *c)
{
    QComboBox::setCompleter(c);

    if(c)
        connect(completer(), SIGNAL(activated(const QModelIndex &)), SLOT(changeCurrentIndexFromCompleter(QModelIndex)));
    else
        disconnect(completer(), SIGNAL(activated(const QModelIndex &)), this, SLOT (changeCurrentIndexFromCompleter(QModelIndex)));


}

void AdvComboBox::setEditable(bool editable)
{
    QComboBox::setEditable(editable);

    if (editable) {
        setLineEdit(new AdvLineEdit(this));
    } else {
        delete lineEdit();
    }
}

void AdvComboBox::onIndexChanged()
{
    if (isEditable()) {
        lineEdit()->setCursorPosition(0);
        lineEdit()->selectAll();
    }

    QVariant key = model()->index(currentIndex(), m_keyCol).data(m_keyColRole);
    if (m_currentKey != key) {
        m_currentKey = key;
        emit keyChanged();
        emit keyChanged(key);
    }
}

void AdvComboBox::changeCurrentIndexFromCompleter(const QModelIndex &index)
{
    QAbstractProxyModel* completionModel = qobject_cast<QAbstractProxyModel*>(completer()->completionModel());

    if (completionModel)
    {
        int row = completionModel->mapToSource(index).row();
        setCurrentIndex(row);
    }
}

void AdvComboBox::onCurrentTextChanged()
{
    if (currentText().isEmpty()) {
        setCurrentIndex(-1);
    }
}
