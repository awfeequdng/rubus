#ifndef CONTRACTORTYPEMODEL_H
#define CONTRACTORTYPEMODEL_H

#include "advitemmodel.h"

class ContractorTypeModel : public AdvItemModel
{
    Q_OBJECT
public:
    enum Cols {
        IdCol = 0,
        NameCol = 1
    };

    explicit ContractorTypeModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
signals:

public slots:

private:
    QList<int> m_types;
};

#endif // CONTRACTORTYPEMODEL_H
