#ifndef STANDARDTABLEDIALOG_H
#define STANDARDTABLEDIALOG_H

#include <QDialog>

namespace Ui {
class StandardTableDialog;
}

class StandardTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StandardTableDialog(QWidget *parent = 0);
    ~StandardTableDialog();

private:
    Ui::StandardTableDialog *ui;
};

#endif // STANDARDTABLEDIALOG_H
