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
class QSqlQueryModel;

class OperatorTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OperatorTableWidget(QWidget *parent = 0);
    ~OperatorTableWidget();

    void showEvent(QShowEvent *e);
    int currentId() const;
    int currentLocation() const;

public slots:
    void add();
    void editCurrent();
    void removeSelected();

    void populate();
    void popalateLocation();
    void export32();

    void filter();


private:
    Ui::OperatorTableWidget *ui;

    OperatorModel *m_model;
    QSortFilterProxyModel *m_proxyModel;
    OperatorEditWidget *m_editWidget;
    EditDialog *m_editDlg;
    QSqlQueryModel *m_locationModel;
    QSqlQueryModel *m_jobModel;
    QSqlQueryModel *m_shiftModel;

    QAction *m_acAdd;
    QAction *m_acEdit;
    QAction *m_acRemove;

    QAction *m_export;

    QString selectedIds() const;
    QDate dateFromString(const QString &string);
};

#endif // OPERATORTABLEWIDGET_H
