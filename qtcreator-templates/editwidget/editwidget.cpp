#include "%EditWidgetClassName:l%.%CppHeaderSuffix%"
#include "ui_%EditWidgetClassName:l%.%CppHeaderSuffix%"

#include <QtSql>
#include <QDebug>

%EditWidgetClassName%::%EditWidgetClassName%(QWidget *parent) :
    EditWidgetInterface(parent),
    ui(new Ui::%EditWidgetClassName%)
{
    ui->setupUi(this);

    connect(ui->btnSave, SIGNAL(clicked()), SLOT(save()));
    connect(ui->btnCancel, SIGNAL(clicked()), SIGNAL(rejected()));
}

%EditWidgetClassName%::~%EditWidgetClassName%()
{
    delete ui;
}


QVariant %EditWidgetClassName%::id() const
{
    return m_id;
}

bool %EditWidgetClassName%::load(QVariant id)
{
    m_id = id.isValid() ? id.toInt() : -1;

    if (m_id != -1) {
        QSqlQuery sql;
        sql.exec(QString("")
                 .arg(m_id));

        if (sql.lastError().isValid()) {
            setErrorString(sql.lastError().text());
            qCritical() << sql.lastError();
            return false;
        }

        if (!sql.next()) {
             setErrorString(tr("Item #%1 not found!").arg(m_id));
             return false;
        }
    }

    setWindowTitle(m_id == -1 ? tr("New item") : tr("Edit  item #%1").arg(m_id));
    return true;
}

bool %EditWidgetClassName%::save()
{
    QSqlQuery sql;

    if (m_id == -1) {
        sql.prepare("INSERT INTO _table_ () VALUES ()");
    } else {
        sql.prepare("UPDATE _table_ SET  WHERE _id = :id");
        sql.bindValue(":id", m_id);
    }

    if (!sql.exec()) {
        setErrorString(sql.lastError().text());
        qCritical() << sql.lastError();
        return false;
    }

    if (m_id == -1) {
        sql.exec("SELECT currval(pg_get_serial_sequence('_table_', '_id'))");

        if (sql.next()) {
            m_id = sql.value(0).toInt();
        } else {
            qWarning() << sql.lastError();
        }
    }

    emit saved();
    return true;
}
