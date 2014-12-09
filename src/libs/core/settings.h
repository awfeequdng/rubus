#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

#include "core_global.h"

class CORE_EXPORT Settings : public QSettings
{
    Q_OBJECT
public:

    Settings ( Scope scope, const QString & organization, const QString & application = QString(), QObject * parent = 0 );
    Settings ( const QString & fileName, Format format, QObject * parent = 0 );
    Q_INVOKABLE void setValue(const QString & key, const QVariant & value);
    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

signals:

public slots:

};

#endif // SETTINGS_H
