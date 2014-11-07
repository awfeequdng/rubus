#ifndef ITEMEDITWIDGET_H
#define ITEMEDITWIDGET_H

#include <QWidget>
#include "widgets/editwidgetinterface.h"

namespace Ui {
class ItemEditWidget;
}

class ItemTypeModel;
class QSqlQueryModel;

class ItemEditWidget : public EditWidgetInterface
{
    Q_OBJECT

public:
    explicit ItemEditWidget(QWidget *parent = 0);
    ~ItemEditWidget();

    QVariant id() const { return m_id; }

    bool load(QVariant id);
    bool save();

private:
    Ui::ItemEditWidget *ui;
    ItemTypeModel *m_typeModel;
    QSqlQueryModel *m_unitModel;

    int m_id;
};

#endif // ITEMEDITWIDGET_H
