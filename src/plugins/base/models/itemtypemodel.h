#ifndef ITEMTYPEMODEL_H
#define ITEMTYPEMODEL_H

#include <advitemmodel.h>

class ItemTypeModel : public AdvItemModel
{
    Q_OBJECT
public:
    enum Cols {
        IdCol = 0,
        NameCol = 1
    };

    explicit ItemTypeModel(QObject *parent = 0);

signals:

public slots:


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QMap<QString, QString> m_nameById;
    QList<QString> m_items;
};

#endif // ITEMTYPEMODEL_H
