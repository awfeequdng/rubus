#ifndef %TableClassName:u%_H
#define %TableClassName:u%_H

#include "widgets/tabledialog.h"
#include "report.h"

namespace Ui {
class %TableClassName%;
}

class %ModelClassName%;
class %EditWidgetClassName%;

class %TableClassName% : public QDialog
{
    Q_OBJECT
public:
    explicit %TableClassName%(QWidget *parent = 0);
    ~%TableClassName%();

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
    Ui::%TableClassName% *ui;
    %ModelClassName% *m_model;
    QSortFilterProxyModel *m_proxyModel;
    %EditWidgetClassName% *m_editWdg;
    EditDialog *m_editDialog;
    QPoint m_pos;

private:    
    QList<int> sourceRowsFromProxy(QModelIndexList indexes) const;

};

#endif //%TableClassName:u%_H
