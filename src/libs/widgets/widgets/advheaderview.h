#ifndef HEADERVIEW_H
#define HEADERVIEW_H

#include <QHeaderView>
#include "widgets_global.h"

class WIDGETS_EXPORT AdvHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit AdvHeaderView(QWidget *parent = 0);

    void setModel(QAbstractItemModel *model);

    void resizeSection(int logicalIndex, int size);

    void setDefaultSectionSize(int section, int size);
    int defaultSectionSize(int section) const;

    void setSectionLocked(int section, bool lock);
    bool isSectionLocked(int section) const;


    QByteArray saveGeometry() const;
    bool restoreGeometry(const QByteArray &geometry);
signals:

private slots:
    void onSectionResized ( int logicalIndex, int oldSize, int newSize );
    void slotCustomContextMenuRequested(const QPoint &p);
    void resetWidth();
    void resetAllWidths();
    void popupMenuActionTriggered(QAction *pAction);

private:
    bool m_resizingInProcess;

   QMap<int, bool> m_colStretch;
   QMap<int, int> m_savedSize;
   QMap<int, int> m_defaultSectionSize;
   QMap<int, int> m_lockedSection;
   int m_resetWhichWidth;
   QMenu *m_menu;

protected:
   void resizeEvent(QResizeEvent *e);

private:
   bool colIsStretch(int index) const;

   void resizeSections();

};

#endif // HEADERVIEW_H
