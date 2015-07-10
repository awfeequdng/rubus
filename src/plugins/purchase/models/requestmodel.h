#ifndef REQUESTMODEL_H
#define REQUESTMODEL_H

#include "advitemmodel.h"
#include <QSet>

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

    void setLocation(int location);
    void setStates(QSet<int> states);

    bool populate();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

    static QString stateName(int state);

public slots:
    bool submit();

private:
    QSqlQuery *m_sql;
    int m_location;
    QSet<int> m_states;

    void addWhere(QString &where, const QString added);
};

#endif // REQUESTMODEL_H
