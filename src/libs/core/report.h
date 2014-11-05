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
#ifndef REPORT_H
#define REPORT_H

#include <QVariant>
#include <QObject>
#include "core_global.h"

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

class CORE_EXPORT Report
{
public:
    enum Engine {
        OpenOfficeEngine = 0,
        NcReportEngine = 1,
        CuteReportEngine = 2
    };

    explicit Report();
    Report(int engine);
    void setId(int id);
    int id() const;

    QString title() const;
    void setTitle(QString title);

    QString menuId() const;
    void setMenuId(QString menu);

    QString filename() const;
    void setFilename(QString name);

    int engine() const;

    void appendModel(QAbstractItemModel *model);
    QAbstractItemModel *modelAt(int i) const;
    QList<QAbstractItemModel *> models() const;
    void clearModels();

    void appendParametr(QString name, QVariant value);
    QVariant parametrValue(QString paramName) const;
    QHash<QString, QVariant> paramentrs() const;

    bool isValid() const;

private:
    int m_id;
    QString m_title;
    int m_engine;
    QString m_menuId;
    QString m_filename;
    QList<QAbstractItemModel *> m_models;
    QHash<QString, QVariant> m_params;

};

Q_DECLARE_METATYPE(Report)

#endif
