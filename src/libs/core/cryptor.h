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
#ifndef CRYPTOR_H
#define CRYPTOR_H

#include <QString>
#include <QObject>

#include "core_global.h"

class CORE_EXPORT Cryptor : public QObject
{
    Q_OBJECT
    static const quint64 key = 8374568;
public:

    Q_INVOKABLE static QString encode(QString value)
    {
        QByteArray arr(value.toUtf8());
        for(int i =0; i<arr.size(); i++)
                arr[i] = arr[i] ^ key;

        return QString::fromUtf8(arr.toBase64());
    }

    Q_INVOKABLE static QString decode(QString value)
    {
        QByteArray arr = QByteArray::fromBase64(value.toUtf8());
        for(int i =0; i<arr.size(); i++)
                arr[i] =arr[i] ^ key;

        return QString::fromUtf8(arr);
    }
};


#endif // CRYPTOR_H


