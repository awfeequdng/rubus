#ifndef TREECOMBOBOX_H
#define TREECOMBOBOX_H

#include <QComboBox>
#include "widgets_global.h"

class WIDGETS_EXPORT TreeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit TreeComboBox(QWidget *parent = 0);

    bool eventFilter(QObject* object, QEvent* event);
    virtual void showPopup();
    virtual void hidePopup();

    void setCurrentIndex(int index);
    void setCurrentIndex(QModelIndex &index);

private:
    bool skipNextHide;
    int m_keyCol;
    QVariant m_currentKey;
    int m_displayCol;


public slots:

};

#endif // TREECOMBOBOX_H
