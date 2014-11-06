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
#include "doublespinboxdelegate.h"

#include <QPainter>
#include <QApplication>
#include <QtGui>

#include "widgets/doublespinbox.h"

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(Qt::Alignment align)
{
    m_align = align;
    m_maxValue = 9999999999.99;
    m_minValue = -9999999999.99;

}

void DoubleSpinBoxDelegate::setMinimumValue(double min)
{
    m_minValue = min;
}

void DoubleSpinBoxDelegate::setMaximumValue(double max)
{
    m_maxValue = max;
}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    DoubleSpinBox *editor = new DoubleSpinBox(parent);
    editor->setMinimum(m_minValue);
    editor->setMaximum(m_maxValue);
    editor->setDecimals(2);
    editor->setAlignment(m_align);

    return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::DisplayRole).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

void DoubleSpinBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 myOption = option;
    QString text = QString::number(index.data().toDouble(),'f',2);

    QRegExp ex("[.,]0{2}");
    if (ex.indexIn(text) != -1)
        text.remove(QRegExp("[.,]\\d+"));

    QRegExp rx("[.,]");
    int cnt = (rx.indexIn(text) != -1)
            ? text.indexOf(rx)
            : text.length();

    while (cnt > 3) {
        cnt -= 3;
        text.insert(cnt," ");
    }

    text.replace(rx,QLocale::system().decimalPoint());

    QVariant background = index.data(Qt::BackgroundRole);
    if (background.canConvert<QBrush>())
        painter->fillRect(option.rect, background.value<QBrush>());

    myOption.displayAlignment = m_align;

    myOption.text = text;
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}
