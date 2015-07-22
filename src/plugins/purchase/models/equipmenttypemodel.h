#ifndef EQUIPMENTTYPEMODEL_H
#define EQUIPMENTTYPEMODEL_H

#include <QAbstractItemModel>
#include <QtGui>

class EquipmentTypeModelItem;

class EquipmentTypeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit EquipmentTypeModel(QObject *parent = 0);

    void populate();

    EquipmentTypeModelItem *itemByIndex(QModelIndex index) const;

    void setReadOnly(bool value) { m_readOnly = value; }
    bool isReadOnly() const { return m_readOnly; }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QModelIndex indexById(int id) const;


private:
    EquipmentTypeModelItem *m_rootItem;
    QIcon m_itemIcon;
    QHash<int, EquipmentTypeModelItem *> m_itemById;
    bool  m_readOnly;
};

#endif // EQUIPMENTTYPEMODEL_H
