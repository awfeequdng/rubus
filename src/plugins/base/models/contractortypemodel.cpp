#include "contractortypemodel.h"
#include "contractor.h"

#include <QDebug>

ContractorTypeModel::ContractorTypeModel(QObject *parent) :
    AdvItemModel(parent)
{
    m_types.append(Contractor::Organization);
    m_types.append(Contractor::Employee);
    m_types.append(Contractor::Maintainer);
}


int ContractorTypeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_types.count();
}

int ContractorTypeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant ContractorTypeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::EditRole || role == Qt::DisplayRole) {
        switch(index.column()) {
        case IdCol : return m_types.at(index.row());
        case NameCol : return Contractor::nameByType(m_types.at(index.row()));
        }
    }

    return AdvItemModel::data(index, role);
}
