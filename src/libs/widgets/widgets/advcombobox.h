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
#ifndef ADVCOMBOBOX_H
#define ADVCOMBOBOX_H

#include <QComboBox>
#include "widgets_global.h"

class AdvLineEdit;

class WIDGETS_EXPORT AdvComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit AdvComboBox(QWidget *parent = 0);

    void setModel(QAbstractItemModel *model, int keyCol, int displayCol);

    void setCurrentKey(QVariant key);
    QVariant currentKey() const;

    void setCompleter(QCompleter *c);
    void setEditable(bool editable);

signals:
    void keyChanged();
    void keyChanged(QVariant id);

private slots:
    void onIndexChanged();
    void changeCurrentIndexFromCompleter(const QModelIndex & index);
    void onCurrentTextChanged();

private:
    int m_keyCol;
    int m_displayCol;

    int m_keyColRole;
    int m_displayColRole;

    QVariant m_currentKey;
    AdvLineEdit *m_lineEdit;

    QAbstractItemModel *m_model;

    void setCompleterChangesCurrentIndex(bool change);

};

#endif // ADVCOMBOBOX_H
