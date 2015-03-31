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

    enum OperatorStates {
        AllState = -1,
        EducationgState = 0,
        AttestatedState = 1,
        RemovedState = 2
    };

    explicit OperatorModel(QObject *parent = 0);

    bool populate();
    void setLocation(int location);
    void setOperatorState(int state);

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
    int m_state;

    QString dateToString(const QDate &date) const;
    void addWhere(QString &where, const QString &added);
};

#endif //  OPERATORMODEL_H

