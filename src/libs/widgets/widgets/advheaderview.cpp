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
    Q_UNUSED(oldSize)

    if (!model())
        return;

    if (newSize <= 0) {
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

bool AdvHeaderView::colIsStretch(int index) const
{
    return m_colStretch.value(index, true);
}


void AdvHeaderView::setModel(QAbstractItemModel *model)
{
    QHeaderView::setModel(model);

    for (int i = 0; i < count(); i++) {
        resizeSection(i, m_savedSize.value(i));
    }
}

void AdvHeaderView::resizeSection(int logicalIndex, int size)
{
    QHeaderView::resizeSection(logicalIndex, size);

    if (!m_resizingInProcess) {
        m_colStretch.insert(logicalIndex, size <= 0);
    }

}

QByteArray AdvHeaderView::saveGeometry() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << int(count());

    for (int i = 0; i < count(); i++) {
        if (colIsStretch(i)) {
            stream << int(-1);
        } else {
            stream << int(sectionSize(i));
        }
    }

    return data;
}

bool AdvHeaderView::restoreGeometry(const QByteArray &geometry)
{
    if (geometry.isEmpty()) {
        return false;
    }

    QByteArray data = geometry;
    QDataStream stream(&data, QIODevice::ReadOnly);

    int sectionCount = 0;
    stream >> sectionCount;

    if (sectionCount != count()) {
        return false;
    }

    for (int i = 0; i < count(); i++) {
        int size;
        stream >> size;
        m_savedSize.insert(i, size);
    }

    return true;
}
