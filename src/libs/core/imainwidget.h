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
#ifndef IMAINWIDGET_H
#define IMAINWIDGET_H


#include <QWidget>

#include "core_global.h"
#include "iplugin.h"

namespace Core {

class CORE_EXPORT IMainWidget : public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(Core::IPlugin)
public:
    explicit IMainWidget(QObject *parent = 0);

    virtual QList<QAction *> toolBarActions() const = 0;
    virtual QIcon icon() const = 0;
    virtual QWidget* widget() const = 0;

signals:

public slots:

private:

};
} //namespace Core

Q_DECLARE_INTERFACE(Core::IMainWidget,
                     "Rubus.IMainWindow/1.0")

#endif // MAINWIDGETINTERFACE_H
