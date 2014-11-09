#include "advtableview.h"
#include "advheaderview.h"

AdvTableView::AdvTableView(QWidget *parent) :
    QTableView(parent),
    m_horisontalHeader(new AdvHeaderView(this))
{
    setHorizontalHeader(m_horisontalHeader);
}


void AdvTableView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
}
