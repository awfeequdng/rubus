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
signals:

private slots:
    void onSectionResized ( int logicalIndex, int oldSize, int newSize );

private:
    bool m_resizingInProcess;

   QMap<int, bool> m_colStretch;

protected:
   void resizeEvent(QResizeEvent *e);

private:
   bool colIsStretch(int index);

};

#endif // HEADERVIEW_H
