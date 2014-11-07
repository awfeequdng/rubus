#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include "advitemmodel.h"

namespace Internal {

struct Item {
    int id;
    QString name;
    QString article;
    int typeId;
    QString typeName;
    QString unitId;
    QString unitTitle;
    bool active;
};
}

class ItemModel : public AdvItemModel
{
    Q_OBJECT
public:
    enum Cols {
        IdCol = 0,
        NameCol = 1,
        ArticleCol = 2,
        TypeCol = 3,
        UnitCol = 4
    };

    explicit ItemModel(QObject *parent = 0);
    bool populate();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool removeRows(int row, int count, const QModelIndex &parent);

private:
    QList<Internal::Item *> m_items;
};

#endif // ITEMMODEL_H
