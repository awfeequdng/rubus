#ifndef CONTRACTORMODEL_H
#define CONTRACTORMODEL_H

#include "advitemmodel.h"

namespace Internal {
    struct Item {
        int id;
        QString name;
        int type;
        QString typeName;
    };
}

class ContractorModel : public AdvItemModel
{
    Q_OBJECT
public:
    enum Cols {
        IdCol = 0,
        NameCol = 1,
        TypeCol = 2
    };

    explicit ContractorModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool removeRows(int row, int count, const QModelIndex &parent);

signals:

public slots:
    bool submit();

private:
    QList<Internal::Item *> m_items;
};

#endif // CONTRACTORMODEL_H
