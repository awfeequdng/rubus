#ifndef CONTRACTOR_H
#define CONTRACTOR_H

#include <QQuickItem>

class Contractor : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(Contractor)

public:
    Contractor(QQuickItem *parent = 0);
    ~Contractor();
};

#endif // CONTRACTOR_H

