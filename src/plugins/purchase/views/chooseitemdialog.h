#ifndef CHOOSEITEMDIALOG_H
#define CHOOSEITEMDIALOG_H

#include <QDialog>

namespace Ui {
class ChooseItemDialog;
}

class QSqlQueryModel;
class QSortFilterProxyModel;
class EquipmentTypeModel;

class ChooseItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseItemDialog(QWidget *parent = 0);
    ~ChooseItemDialog();

    void populate();
    int exec(int itemId = -1);

    int selectedItemId() const;
    int selectedEquipmentType() const;

private:
    Ui::ChooseItemDialog *ui;
    QSqlQueryModel *m_itemModel;
    QSortFilterProxyModel *m_itemProxyModel;
    EquipmentTypeModel *m_eqTypeModel;

    int m_eqId;
    int m_itemId;

private slots:
    void appendFilter();
};

#endif // CHOOSEITEMDIALOG_H
