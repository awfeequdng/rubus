#include "settings.h"
#include <QApplication>

Settings::Settings(QSettings::Scope scope,
                   const QString &/*organization*/,
                   const QString &/*application*/,
                   QObject *parent) :
    QSettings(scope, QApplication::organizationName(), QApplication::applicationName(), parent)
{
}

Settings::Settings(const QString &fileName, QSettings::Format format, QObject *parent) :
    QSettings(fileName,format,parent)
{

}

void Settings::setValue(const QString &key, const QVariant &value)
{
    QSettings::setValue(key, value);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue) const
{
    return QSettings::value(key, defaultValue);
}
