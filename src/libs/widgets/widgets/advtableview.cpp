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


void AdvTableView::setModel(QAbstractItemModel *model, int keyColumn, int keyRole)
{
    m_keyColumn = keyColumn;
    m_keyRole = keyRole;
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

QVariant AdvTableView::currentId() const
{
    if (!currentIndex().isValid()) {
        return QVariant();
    }

    return model()->index(currentIndex().row(), m_keyColumn).data(m_keyRole);
}

void AdvTableView::setCurrentId(const QVariant &id)
{
    if (!model()) {
        return;
    }

    for (int i = 0; i < model()->rowCount(); i++) {
        if (model()->index(i, m_keyColumn).data(m_keyRole) == id) {
            setCurrentIndex(model()->index(i, m_keyColumn));
            selectRow(i);
        }
    }
}
