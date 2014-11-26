#ifndef %EditWidgetClassName:u%_H
#define %EditWidgetClassName:u%_H

#include <QWidget>
#include "widgets/editwidgetinterface.h"

namespace Ui {
class %EditWidgetClassName%;
}

class %EditWidgetClassName% : public EditWidgetInterface
{
    Q_OBJECT
public:
    explicit %EditWidgetClassName%(QWidget *parent = 0);
    ~%EditWidgetClassName%();

    QVariant id() const;

public slots:
    bool load(QVariant id);
    bool save();
private:
    Ui::%EditWidgetClassName% *ui;
    int m_id;

};

#endif // %EditWidgetClassName:u%_H
