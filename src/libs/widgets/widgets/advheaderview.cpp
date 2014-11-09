#include "advheaderview.h"
#include <QDebug>

AdvHeaderView::AdvHeaderView(QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent),
    m_resizingInProcess(false)
{
    connect(this, SIGNAL(sectionResized(int,int,int)), SLOT(onSectionResized(int,int,int)));
}

void AdvHeaderView::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    qDebug() << logicalIndex;
    Q_UNUSED(oldSize)

    if (!model())
        return;


    if (newSize < 0) {
        m_colStretch.insert(logicalIndex, true);
    }
    else if (! m_resizingInProcess && colIsStretch(logicalIndex)) {
        m_colStretch.insert(logicalIndex, false);
    }


    if (m_resizingInProcess)
        return;

    m_resizingInProcess = true;

    int usedSpace = 0;
    QVector<int> stretch;

    for(int i = 0; i < count(); i++)
    {
        if (isSectionHidden(i))
            continue;
        else if (logicalIndex == i && newSize < 0)
            stretch.append(i);
        else if (logicalIndex == i)
            usedSpace += newSize;
        else if (!colIsStretch(i))
            usedSpace += sectionSize(i);
        else
            stretch.append(i);
    }

    qDebug() << stretch << usedSpace;

    if(stretch.size() > 0)
    {
        int leftover = (viewport()->width() - usedSpace) / stretch.size();

        if(leftover < 50)
            leftover = 50;

        for (int n = 0; n < stretch.size(); n ++)
            resizeSection(stretch.at(n), leftover);
    }

    m_resizingInProcess = false;
}

void AdvHeaderView::resizeEvent(QResizeEvent *e)
{
    QHeaderView::resizeEvent(e);
    onSectionResized(-1, 0, 0);
}

bool AdvHeaderView::colIsStretch(int index)
{
    return m_colStretch.value(index, true);
}


void AdvHeaderView::setModel(QAbstractItemModel *model)
{
    QHeaderView::setModel(model);
}

void AdvHeaderView::resizeSection(int logicalIndex, int size)
{
    m_colStretch.insert(logicalIndex, size <= 0);

    QHeaderView::resizeSection(logicalIndex, size);
}
