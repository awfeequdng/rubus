#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QVariant>

class Field;

struct SourceData {
    QObject *object;
    QString property;
};

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

    QVariant objectId() const;

    void setSchemeObject(const QString &objectName);
    QString schemeObject() const;

    void setUi(QObject *ui);
    QObject *ui() const;

    void bindData(QObject *source, const QString &propertyName, const QString &field);


    QString errorString() const;

signals:

public slots:
    bool load(const QVariant &id = QVariant());
    bool save(const QVariant &id = QVariant());

private:
    QString m_scheme;
    QString m_schemeObject;
    QObject *m_ui;
    QVariant m_id;
    QString m_errorString;

    QVariantMap m_values;
    QMap<QString, Field *> m_fields;
    QMap<QString, SourceData> m_binding;

private:
    void parseScheme(const QString &object);

    QVariant data(const QString &field);
    void setData(const QString &field, const QVariant &value);


};

#endif // CONTROLLER_H
