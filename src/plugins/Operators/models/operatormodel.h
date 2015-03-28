#ifndef OPERATORMODEL_H
#define OPERATORMODEL_H

#include "advitemmodel.h"

class QSqlQuery;

class OperatorModel : public AdvItemModel
{
    Q_OBJECT
public:
    enum Cols {
        IdCol = 0,
        DownCol,
        NameCol,
        BeginEducCol,
        DateAttestCol,
        DateRemoveCol,
        JobCol,
        ShiftCol,
        AttestatorCol
    };

    explicit OperatorModel(QObject *parent = 0);

    bool populate();
    void setLocation(int location);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool removeRows(int row, int count, const QModelIndex &parent);

signals:

public slots:

private:
    QSqlQuery *m_sql;
    QString m_removedIds;
    int m_location;

    QString dateToString(const QDate &date) const;
};

#endif //  OPERATORMODEL_H

