#ifndef OPERATORSPLUGIN_H
#define OPERATORSPLUGIN_H

#include "operators_global.h"
#include "iplugin.h"

class OperatorTableWidget;

class OPERATORS_EXPORT OperatorsPlugin : public Core::IPlugin
{
    Q_OBJECT
    Q_INTERFACES(Core::IPlugin)
#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "OperatorsPlugin")
#endif

public:
    explicit OperatorsPlugin(QObject *parent = 0);
    ~OperatorsPlugin();

    static OperatorsPlugin *instance();

    QString name() const;
    int version() const;
    bool initialize();

public slots:

private:
    OperatorTableWidget *m_operatorTableWdg;

};

#endif // OPERATORSPLUGIN_H

