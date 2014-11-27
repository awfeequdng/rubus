#include "%TableClassName:l%.%CppHeaderSuffix%"
#include "ui_%TableClassName:l%.%CppHeaderSuffix%"
#include "%ModelClassName:l%.%CppHeaderSuffix%"
#include "%EditWidgetClassName:l%.%CppHeaderSuffix%"
#include "reportmanager.h"
#include "widgets/editdialog.h"

#include <QMessageBox>

%TableClassName%::%TableClassName%(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::%TableClassName%)
{
    ui->setupUi(this);

    m_model = new %ModelClassName%(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_editWdg = new %EditWidgetClassName%();
    m_editDialog = new EditDialog(m_editWdg, this);
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterKeyColumn(ContractorModel::NameCol);


    ui->tableView->horizontalHeader()->setDefaultSectionSize(%ModelClassName%::IdCol, 50);
    ui->tableView->setModel(m_proxyModel, %ModelClassName%::IdCol, Qt::DisplayRole);

    connect(ui->btnAdd, SIGNAL(clicked()), SLOT(add()));
    connect(ui->btnEdit, SIGNAL(clicked()), SLOT(editCurrent()));
    connect(ui->btnDelete, SIGNAL(clicked()), SLOT(deleteSelected()));
    connect(ui->btnPrint, SIGNAL(print(Report&)), SLOT(slotPrint(Report&)));
    connect(ui->edFind, SIGNAL(textEdited(QString)), m_proxyModel, SLOT(setFilterRegExp(QString)));
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(viewDoubleClicked(QModelIndex)));

    restoreSettings();
}

%TableClassName%::~%TableClassName%()
{
    saveSettings();
    delete ui;
}

AdvTableView *%TableClassName%::view() const
{
    return ui->tableView;
}

void %TableClassName%::saveSettings(const QString &prefix)
{
    QString p = prefix.isEmpty() ? objectName() : prefix;

    if (p.isEmpty()) {
        qDebug() << "Opps, setting prefix is empty!";
        return;
    }

    QSettings sett;
    sett.setValue(p + "/viewState", view()->horizontalHeader()->saveState());
    sett.setValue(p + "/viewGeometry", view()->saveHeaderGeometry());
    sett.setValue(p + "/pos", m_pos);
    sett.setValue(p + "/size", size());
}


void %TableClassName%::restoreSettings(const QString &prefix)
{
    QString p = prefix.isEmpty() ? objectName() : prefix;

    if (p.isEmpty()) {
        qDebug() << "Opps, setting prefix is empty!";
        return;
    }

    QSettings sett;
    view()->horizontalHeader()->restoreState(sett.value(p + "/viewState").toByteArray());
    view()->restoreHeaderGeometry(sett.value(p + "/viewGeometry").toByteArray());
    move(sett.value(p + "/pos").toPoint());
    resize(sett.value(p + "/size").toSize());
}

void %TableClassName%::add()
{
    if (m_editDialog->exec() == QDialog::Accepted) {
        m_model->populate();
        view()->setCurrentId(m_editWdg->id());
        view()->setFocus();
    }
}

void %TableClassName%::editCurrent()
{
    if (m_editDialog->exec(view()->currentId()) == QDialog::Accepted) {
        m_model->populate();
        view()->setFocus();

        view()->setCurrentId(m_editWdg->id());
        view()->setFocus();
    }
}

void %TableClassName%::deleteSelected()
{
    QModelIndexList rows = view()->selectionModel()->selectedRows();

    if (rows.isEmpty()) {
        return;
    }

    if (QMessageBox::warning(this,
                             tr("Delete"),
                             tr("Are you sure you want to remove selected items(%1)").arg(rows.count()),
                             QMessageBox::Cancel | QMessageBox::Ok)
            == QMessageBox::Cancel) {
        return;
    }

    QListIterator<int> iter(sourceRowsFromProxy(rows));
    iter.toBack();
    while(iter.hasPrevious()) {
        if (!m_model->removeRow(iter.previous())) {
            m_model->revert();
            QMessageBox::critical(this, tr("Delete"), m_model->errorString());
            return;
        }
    }

    if (!m_model->submit()) {
        QMessageBox::critical(this, tr("Delete"), m_model->errorString());
    }
}

void %TableClassName%::slotPrint(Report &r)
{
    r.appendModel(m_model);
    Core::ReportManager::showReport(r);
}

void %TableClassName%::viewDoubleClicked(QModelIndex index)
{
    if (index.flags() & Qt::ItemIsEditable) {
        return;
    }

    editCurrent();
}

void %TableClassName%::showEvent(QShowEvent *e)
{
    if (!m_model->populate()) {
        QMessageBox::critical(this, tr("Error"), m_model->errorString());
    }
    QDialog::showEvent(e);
}

void %TableClassName%::hideEvent(QHideEvent *e)
{
    m_pos = pos();
    QDialog::hideEvent(e);
}

QList<int> %TableClassName%::sourceRowsFromProxy(QModelIndexList indexes) const
{
    QListIterator<QModelIndex> iter(indexes);
    QList<int> rows;

    while(iter.hasNext()) {
        rows << m_proxyModel->mapToSource(iter.next()).row();
    }

    qSort(rows);
    return rows;
}
