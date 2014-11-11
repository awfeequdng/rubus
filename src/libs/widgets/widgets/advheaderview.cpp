#include "advheaderview.h"

#include <QDebug>
#include <QMenu>

AdvHeaderView::AdvHeaderView(QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent),
    m_resizingInProcess(false),
    m_resetWhichWidth(-1)
{
    m_menu = new QMenu(this);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(sectionResized(int,int,int)), SLOT(onSectionResized(int,int,int)));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(slotCustomContextMenuRequested(QPoint)));
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

void AdvHeaderView::slotCustomContextMenuRequested(const QPoint &p)
{
    m_menu->clear();

    int logicalIndex = logicalIndexAt(p);
    int currentSize = sectionSize(logicalIndex);

    if (defaultSectionSize(logicalIndex) > 0 && defaultSectionSize(logicalIndex) != currentSize )
    {
        m_resetWhichWidth = logicalIndex;
        m_menu->addAction(tr("Reset width this column"), this, SLOT(resetWidth()));
    }

    m_menu->addAction(tr("Reset width all column"), this, SLOT(resetAllWidths()));
    m_menu->addSeparator();

    for(int i = 0; i < count(); i++)
    {
        QAction *act = m_menu->addAction(model()->headerData(i, Qt::Horizontal).toString());
        act->setCheckable(true);
        act->setChecked(!isSectionHidden(i));

        //act->setVisible(tmp ?  ! tmp->locked() : true);

        QMap<QString,QVariant> m;
        m.insert("command", QVariant("toggleColumnHidden"));
        m.insert("column", QVariant(i));
        act->setData(m);
        connect(m_menu, SIGNAL(triggered(QAction*)), this, SLOT(popupMenuActionTriggered(QAction*)));
    }

    if(! m_menu->isEmpty())
        m_menu->popup(mapToGlobal(p));
}

void AdvHeaderView::resetWidth()
{
    if (m_resetWhichWidth < 0) {
        return;
    }

    resizeSection(m_resetWhichWidth, defaultSectionSize(m_resetWhichWidth));
}

void AdvHeaderView::resetAllWidths()
{
    bool autoSections = false;
    for (int i = 0; i < length(); i++)
    {
        int width = defaultSectionSize(i);
        if (width >= 0) {
            resizeSection(i, width);
        } else
        {
            m_colStretch.insert(i, true);
            autoSections = true;
        }
    }

    if(autoSections)
        onSectionResized(-1, 0, 0);
}

void AdvHeaderView::popupMenuActionTriggered(QAction *pAction)
{
    QMap<QString, QVariant> m = pAction->data().toMap();
    QString command = m.value("command").toString();
    if("toggleColumnHidden" == command) {
        setSectionHidden(m.value("column").toInt(), !pAction->isChecked());
    }
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
        resizeSection(i, !m_savedSize.isEmpty()
                      ? m_savedSize.value(i)
                      : m_defaultSectionSize.value(i));

    }
}

void AdvHeaderView::resizeSection(int logicalIndex, int size)
{
    QHeaderView::resizeSection(logicalIndex, size);

    if (!m_resizingInProcess) {
        m_colStretch.insert(logicalIndex, size <= 0);
    }

}

void AdvHeaderView::setDefaultSectionSize(int section, int size)
{
    m_defaultSectionSize.insert(section, size);
}

int AdvHeaderView::defaultSectionSize(int section) const
{
    return m_defaultSectionSize.value(section, -1);
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
