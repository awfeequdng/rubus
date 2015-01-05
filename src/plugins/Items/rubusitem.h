#ifndef RUBUSITEM_H
#define RUBUSITEM_H

#include <QQuickItem>

class RubusItem : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(RubusItem)

public:
    RubusItem(QQuickItem *parent = 0);
    ~RubusItem();
};

#endif // RUBUSITEM_H

