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
#ifndef TABLEEDITINTERFACE_H
#define TABLEEDITINTERFACE_H

#include <QWidget>
#include <QtGui>

#include "widgets_global.h"

class AdvTableView;

class WIDGETS_EXPORT TableEditInterface : public QWidget
{
    Q_OBJECT
public:

    explicit TableEditInterface(QWidget *parent = 0);

    virtual QVariant currentPrimaryKey() const = 0;
    virtual void setCurrentPrimaryKey(QVariant value) = 0;

    virtual QList<QVariant> selectedPrimaryKeys() const = 0;
    virtual AdvTableView *view() const = 0;


    QString settingPrefix() const { return m_settingPrefix; }
    void setSettingPrefix(QString prefix) { m_settingPrefix = prefix; }

protected:
    QList<int> sourceRowsFromProxy(QModelIndexList indexes, QSortFilterProxyModel *model) const;
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

signals:

public slots:
    virtual void populate() = 0;

private:
    QString m_settingPrefix;

};

#endif // TABLEEDITINTERFACE_H
