#ifndef OPERATORMODEL_H
#define OPERATORMODEL_H

#include "advitemmodel.h"

namespace Internal {
struct Item {
    int id;
    QString name;
};
}

class OperatorModel : public AdvItemModel
{
    Q_OBJECT
public:
    enum Cols {
        IdCol = 0,
        NameCol = 1
    };

    explicit OperatorModel(QObject *parent = 0);

    bool populate();

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
    QString m_removedIds;
};

#endif //  OPERATORMODEL_H

