#ifndef FIELD_H
#define FIELD_H

#include <QObject>
#include <QVariant>
#include <QJsonObject>

class Field : public QObject
{
    Q_OBJECT
public:
    explicit Field(QObject *parent = 0);
    Field(const QString &fieldName, const QString &schemeObject,
          const QJsonObject &scheme = QJsonObject(), QObject *parent = 0);

    void load(const QString &fieldName, const QString &schemeObject, const QJsonObject &scheme = QJsonObject());

    QString name() const;
    void setName(const QString &name);

    QString tableFieldName();
    void setTableFieldName(const QString &name);

    QVariant::Type type() const;
    void setType(QVariant::Type type);

    QString title() const;
    void setTitle(const QString &title);

    QString format() const;
    void setFormat(const QString &format);

    static QVariant::Type typeFromString(const QString &type);
signals:

public slots:

private:
    QString m_name;
    QString m_tableFieldName;
    QVariant::Type m_type;
    QString m_title;
    QString m_format;
    bool m_lookup;

private:

};

#endif // FIELD_H
