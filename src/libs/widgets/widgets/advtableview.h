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

    void setModel(QAbstractItemModel *model, int keyColumn = 0, int keyRole = Qt::DisplayRole);

    AdvHeaderView *horizontalHeader() const;

    QByteArray saveHeaderGeometry() const;
    bool restoreHeaderGeometry(const QByteArray &geometry);

    QVariant currentId() const;
    void setCurrentId(const QVariant &id);

signals:

public slots:

private:
    AdvHeaderView *m_horisontalHeader;
    int m_keyColumn;
    int m_keyRole;
};

#endif // ADVTABLEVIEW_H
