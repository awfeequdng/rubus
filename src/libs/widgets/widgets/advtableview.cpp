#include "advtableview.h"
#include "advheaderview.h"

AdvTableView::AdvTableView(QWidget *parent) :
    QTableView(parent),
    m_horisontalHeader(new AdvHeaderView(this))
{
    //setSortingEnabled(true);
    setHorizontalHeader(m_horisontalHeader);
    verticalHeader()->setDefaultSectionSize(25);

}


void AdvTableView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
}

AdvHeaderView *AdvTableView::horizontalHeader() const
{
    return m_horisontalHeader;
}

QByteArray AdvTableView::saveHeaderGeometry() const
{
    return m_horisontalHeader->saveGeometry();
}

bool AdvTableView::restoreHeaderGeometry(const QByteArray &geometry)
{
    return m_horisontalHeader->restoreGeometry(geometry);
}
