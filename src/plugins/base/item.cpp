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
#include "item.h"

#include <QObject>
#include <QDebug>

Item::Item()
{
}

QString Item::nameType(Item::Types type)
{
    switch(type) {
    case Manufacture : return QObject::tr("Manufacture");
    case Purchase : return QObject::tr("Purchase");
    case Sparepart : return QObject::tr("Sparepart");
    }

    return QObject::tr("Unknown");
}

QString Item::idType(Item::Types type)
{
    switch(type) {
    case Manufacture : return QLatin1String("M");
    case Purchase : return QLatin1String("P");
    case Sparepart : return QLatin1String("S");
    }

    return QLatin1String("");
}

QMap<QString, QString> Item::itemMap()
{
    QMap<QString, QString> m;

    m.insert(idType(Manufacture), nameType(Manufacture));
    m.insert(idType(Purchase), nameType(Purchase));
    m.insert(idType(Sparepart), nameType(Sparepart));

    return m;
}
