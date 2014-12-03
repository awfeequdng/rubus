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
#ifndef USEREDITWIDGET_H
#define USEREDITWIDGET_H

#include <QWidget>
#include "widgets/editwidgetinterface.h"

namespace Ui {
class UserEditWidget;
}

class QSqlQueryModel;

namespace Core {
    class User;
}

class UserEditWidget : public EditWidgetInterface
{
    Q_OBJECT
public:
    explicit UserEditWidget(QWidget *parent = 0);
    ~UserEditWidget();

    QVariant id() const;

public slots:
    bool load(QVariant id);
    bool save();

private:
    Ui::UserEditWidget *ui;
    QString m_role;
    Core::User *m_user;

    QSqlQueryModel *m_contractorModel;

private:
    void populate();
    bool isValid();

};

#endif // USEREDITWIDGET_H

