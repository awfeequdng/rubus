#ifndef %ClassName:u%_H
#define %ClassName:u%_H

#include <QWidget>
#include "widgets/editwidgetinterface.h"

namespace Ui {
class %ClassName%;
}

class %ClassName% : public EditWidgetInterface
{
    Q_OBJECT
public:
    explicit %ClassName%(QWidget *parent = 0);
    ~%ClassName%();

    QVariant id() const;

public slots:
    bool load(QVariant id);
    bool save();
private:
    Ui::%ClassName% *ui;
    int m_id;

};

#endif // %ClassName:u%_H
