#include "sortfiltermodel.h"

SortFilterModel::SortFilterModel()
{

}

SortFilterModel::~SortFilterModel()
{

}

void SortFilterModel::setSource(QAbstractItemModel *model)
{
    QSortFilterProxyModel::setSourceModel(model);
    emit sourceChanged();
}

QAbstractItemModel *SortFilterModel::source() const
{
    return QSortFilterProxyModel::sourceModel();
}

void SortFilterModel::setTest(const QString &name)
{

}

QString SortFilterModel::test() const
{

}

bool SortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)
    Q_UNUSED(source_row)
    return true;
}

bool SortFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return QSortFilterProxyModel::lessThan(left, right);
}

QVariant::Type SortFilterModel::columnType(int col)
{
    Q_UNUSED(col)
    return QVariant::String;
}
