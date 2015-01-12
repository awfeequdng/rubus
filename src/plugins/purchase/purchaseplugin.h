#ifndef PURCHASEPLUGIN_H
#define PURCHASEPLUGIN_H

#include "purchase_global.h"
#include "iplugin.h"

class RequestTableWidget;

class PURCHASE_EXPORT PurchasePlugin : public Core::IPlugin
{
    Q_OBJECT
    Q_INTERFACES(Core::IPlugin)
#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "PurchasePlugin")
#endif

public:
    explicit PurchasePlugin(QObject *parent = 0);
    ~PurchasePlugin();

    static PurchasePlugin *instance();

    QString name() const;
    int version() const;
    bool initialize();

public slots:

private:
    RequestTableWidget *m_requestTableWidget;
};

#endif // PURCHASEPLUGIN_H

