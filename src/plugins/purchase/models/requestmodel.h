#ifndef REQUESTMODEL_H
#define REQUESTMODEL_H

#include "advitemmodel.h"

class QSqlQuery;

class RequestModel : public AdvItemModel
{
    Q_OBJECT
public:
    enum Cols {
        IdCol = 0,
        StateCol,
        DateCol,
        StorageCol,
        EquipmentCol,
        ItemCol,
        QtyCol,
        BalanceCol,
    };

    RequestModel(QObject *parent = 0);

    bool populate();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

public slots:
    bool submit();

private:
    QSqlQuery *m_sql;

    // QAbstractItemModel interface
public:
};

#endif // REQUESTMODEL_H
