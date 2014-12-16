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
#include <QUrl>
#include "core_global.h"

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

class CORE_EXPORT Report : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int reportId READ reportId WRITE setReportId NOTIFY reportIdChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString menu READ menu WRITE setMenu NOTIFY menuChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QUrl fromFile READ fromFile WRITE setFromFile NOTIFY fromFileChanged)

public:
    explicit Report(QObject *parent = 0);

    void setReportId(int reportId);
    int reportId() const;

    QString name() const;
    void setName(QString name);

    QString menu() const;
    void setMenu(QString menu);

    QUrl fromFile() const;
    void setFromFile(const QUrl &file);

    void appendModel(QAbstractItemModel *model);
    QAbstractItemModel *modelAt(int i) const;
    QList<QAbstractItemModel *> models() const;
    void clearModels();

    void appendParametr(QString name, QVariant value);
    QVariant parametrValue(QString paramName) const;
    QHash<QString, QVariant> paramentrs() const;

    Q_INVOKABLE bool load();
    Q_INVOKABLE bool save();

    bool isValid() const;

    Q_INVOKABLE void show();
    Q_INVOKABLE void print(QString printerName, int copies,  bool showDialog);

    QString errorString() const;

signals:
    void reportIdChanged();
    void nameChanged();
    void menuChanged();
    void errorStringChanged();
    void fromFileChanged();
    void saved();
    void loaded();

private:
    int m_id;
    QString m_name;
    QString m_menuId;
    QString m_data;
    QUrl m_fromFile;
    QList<QAbstractItemModel *> m_models;
    QHash<QString, QVariant> m_params;
    QString m_errorString;

    void setError(const QString &error);

};

//Q_DECLARE_METATYPE(Report*)

#endif
