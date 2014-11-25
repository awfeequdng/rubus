#ifndef %ClassName:u%_H
#define %ClassName:u%_H

#include "widgets/tabledialog.h"
#include "report.h"

namespace Ui {
class %ClassName%;
}

class %ModelClass%;
class %EditWidgetClass%;

class %ClassName% : public QDialog
{
    Q_OBJECT
public:
    explicit %ClassName%(QWidget *parent = 0);
    ~%ClassName%();

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
    Ui::%ClassName% *ui;
    %ModelClass% *m_model;
    QSortFilterProxyModel *m_proxyModel;
    %EditWidgetClass% *m_editWdg;
    EditDialog *m_editDialog;
    QPoint m_pos;

private:    
    QList<int> sourceRowsFromProxy(QModelIndexList indexes) const;

};

#endif //%ClassName:u%_H
