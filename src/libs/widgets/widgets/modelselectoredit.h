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
#ifndef MODELSELECTOREDIT_H
#define MODELSELECTOREDIT_H

#include <QWidget>
#include <widgets_global.h>

class AdvLineEdit;
class QLabel;
class QAbstractItemModel;
class AdvComboBox;

class WIDGETS_EXPORT ModelSelectorEdit : public QWidget
{
    Q_OBJECT
public:
    explicit ModelSelectorEdit(QWidget *parent = 0);

    void setVisibleComboBox(bool visible);
    bool isVisibleComboBox() const;

    int currentId() const;
    void setCurrentId(int id);

    void setModel(QAbstractItemModel *model, int idCol = 0, int displayCol = 1);

    bool isValid() const;

signals:
    void editFinished();
    void editReturnPressed();
    void currentIdChanged(int id);

private slots:
    void numberEditFinished();
    void onComboBoxChanged();

    void activateCombo();

private:
    AdvLineEdit *m_edNumber;
    QLabel *m_labName;
    QAbstractItemModel *m_model;
    AdvComboBox *m_comboBox;

    int m_id;
    int m_idCol;
    int m_displayCol;

    bool contains(QVariant id) const;
    QString displayData(QVariant id) const;

};

#endif // MODELSELECTOREDIT_H
