#ifndef EQUIPMENTTYPEMODELITEM_H
#define EQUIPMENTTYPEMODELITEM_H

#include <QObject>

class EquipmentTypeModelItem : public QObject
{
    Q_OBJECT
public:
    explicit EquipmentTypeModelItem();
    EquipmentTypeModelItem(int id, QString name, int parentId, EquipmentTypeModelItem *parent = 0);
    ~EquipmentTypeModelItem();

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    int parentId() const { return m_parentId; }
    void setParentId(int id) { m_parentId = id; }

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }

    int storageId() const { return m_storageId; }
    void setStorageId(int id) { m_storageId = id; }

    bool isChecked() const { return m_checked; }
    void setChecked(bool check) { m_checked = check; }

    void idWithChildrenIndexes(QString &indexes) const;

    void setUsed(bool used);
    bool isUsed() const { return m_used; }

    EquipmentTypeModelItem *parent() const { return m_parent; }
    void setParent(EquipmentTypeModelItem *parent);

    void addChild(EquipmentTypeModelItem *child) { m_children.append(child); }
    const QList<EquipmentTypeModelItem*> children() const { return m_children; }
    int childCount() const { return m_children.count(); }
    void clearChildrenItems();
    EquipmentTypeModelItem *child(int i) const;
    int at() const;

signals:

public slots:
private:
    int m_id;
    int m_parentId;
    QString m_name;
    EquipmentTypeModelItem *m_parent;
    QList<EquipmentTypeModelItem*>  m_children;
    int m_storageId;
    bool m_checked;
    bool m_used;

};

#endif // EQUIPMENTTYPEMODELITEM_H
