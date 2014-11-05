/***************************************************************************
 *   This file is part of the Xdbm project                                 *
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
#include "doublespinbox.h"

#include <QKeyEvent>

#include "advlineedit.h"

DoubleSpinBox::DoubleSpinBox(QWidget *parent) :
    QDoubleSpinBox(parent)
{
    m_lineEdit = new AdvLineEdit(this);
    setLineEdit(m_lineEdit);

    lineEdit()->setValidator(new QRegExpValidator(
                                 QRegExp("-?[0-9]*[.|,]?[0-9]*"), this));

    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Minimum);
    setSizePolicy(policy);

    setMaximum(9999999999.999);
}

QSize DoubleSpinBox::sizeHint() const
{
    QSize size = QDoubleSpinBox::sizeHint();
    size.setWidth(120);
    return size;
}

QSize DoubleSpinBox::minimumSizeHint() const
{
    return QSize(120,20);
}

double DoubleSpinBox::valueFromText(const QString &text) const
{
    QString temp = text;
    temp.replace(QRegExp("[.|,]"), QLocale::system().decimalPoint());
    return temp.toDouble();
}

QValidator::State DoubleSpinBox::validate(QString &input, int &pos) const
{
    Q_UNUSED(input)
    Q_UNUSED(pos)

    return QValidator::Acceptable;
}

void DoubleSpinBox::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
        emit returnPressed();
    }

    QDoubleSpinBox::keyPressEvent(e);
}
