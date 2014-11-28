#ifndef USERTABLEDIALOG_H
#define USERTABLEDIALOG_H

#include "widgets/tabledialog.h"
#include "report.h"

namespace Ui {
class UserTableDialog;
}

class UserModel;
class UserEditWidget;

class UserTableDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UserTableDialog(QWidget *parent = 0);
    ~UserTableDialog();

    AdvTableView *view() const;

    void saveSettings(const QString &prefix = QString());
    void restoreSettings(const QString &prefix = QString());

public slots:
    void add();
    void editCurrent();
    void deleteSelected();
    void slotPrint(Report &r);

private slots:
    void viewDoubleClicked(QModelIndex index);

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private:
    Ui::UserTableDialog *ui;
    UserModel *m_model;
    QSortFilterProxyModel *m_proxyModel;
    UserEditWidget *m_editWdg;
    EditDialog *m_editDialog;
    QPoint m_pos;

private:
    QList<int> sourceRowsFromProxy(QModelIndexList indexes) const;

};

#endif //USERTABLEDIALOG_H

