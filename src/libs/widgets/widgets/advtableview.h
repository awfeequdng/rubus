#ifndef ADVTABLEVIEW_H
#define ADVTABLEVIEW_H

#include <QTableView>
#include "widgets_global.h"
#include "advheaderview.h"

class WIDGETS_EXPORT AdvTableView : public QTableView
{
    Q_OBJECT
public:
    explicit AdvTableView(QWidget *parent = 0);

    void setModel(QAbstractItemModel *model);

    AdvHeaderView *horizontalHeader() const;

    QByteArray saveHeaderGeometry() const;
    bool restoreHeaderGeometry(const QByteArray &geometry);

signals:

public slots:

private:
    AdvHeaderView *m_horisontalHeader;
};

#endif // ADVTABLEVIEW_H
