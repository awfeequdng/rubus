#ifndef SORTFILTERMODEL_H
#define SORTFILTERMODEL_H

#include "core_global.h"

//#include <QWidget>
#include <QSortFilterProxyModel>
#include <QVariant>

class CORE_EXPORT SortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel *source READ source WRITE setSource NOTIFY sourceChanged)
public:
    SortFilterModel();
    ~SortFilterModel();

    void setSource(QAbstractItemModel *model);
    QAbstractItemModel *source() const;

signals:
    void sourceChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    QVariant::Type columnType(int col);
};

#endif // SORTFILTERMODEL_H
