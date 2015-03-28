#ifndef OPERATORTABLEWIDGET_H
#define OPERATORTABLEWIDGET_H

#include <QWidget>

namespace Ui {
class OperatorTableWidget;
}

class OperatorModel;
class QSortFilterProxyModel;
class OperatorEditWidget;
class EditDialog;

class OperatorTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OperatorTableWidget(QWidget *parent = 0);
    ~OperatorTableWidget();

    void showEvent(QShowEvent *e);
    int currentId() const;

public slots:
    void add();
    void editCurrent();
    void removeSelected();

    void populate();

private:
    Ui::OperatorTableWidget *ui;

    OperatorModel *m_model;
    QSortFilterProxyModel *m_proxyModel;
    OperatorEditWidget *m_editWidget;
    EditDialog *m_editDlg;

    QAction *m_acAdd;
    QAction *m_acEdit;
    QAction *m_acRemove;

};

#endif // OPERATORTABLEWIDGET_H
