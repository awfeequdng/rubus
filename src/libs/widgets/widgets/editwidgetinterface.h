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
#ifndef EDITWIDGETINTERFACE_H
#define EDITWIDGETINTERFACE_H

#include <QtGui>

#if QT_VERSION < 0x050000
#include <QWidget>
#else
#include <QtWidgets/QWidget>
#endif
#include <QVariant>

#include "widgets_global.h"

class WIDGETS_EXPORT EditWidgetInterface : public QWidget
{
    Q_OBJECT
public:
    explicit EditWidgetInterface(QWidget *parent = 0);

    virtual QVariant id() const { return m_id; }
    virtual void setId(QVariant id) { m_id = id; }

    virtual bool isDataChanged() const { return m_changed; }

    bool isReadOnly() const { return m_readOnly; }
    void setReadOnly(bool readOnly) { m_readOnly = readOnly; }

    QString lastError() const;

    QString settingPrefix() const { return m_settingPrefix; }
    void setSettingPrefix(QString prefix) { m_settingPrefix = prefix; }

    void setWindowTitle ( const QString & s);

signals:
    void windowTitleChanged(QString title);
    void dataChanged();
    void saved();
    void loaded();

public slots:

    virtual bool load(QVariant id = QVariant()) = 0;
    virtual bool save() = 0;

protected:
    void setLastError(QString lastError);

protected slots:
    void setDataChanged(bool changed = true) { m_changed = changed; emit dataChanged(); }


private:
    QString m_lastError;
    QString m_settingPrefix;

    QVariant m_id;
    bool m_changed;
    bool m_readOnly;

};
#endif // EDITWIDGETINTERFACE_H
