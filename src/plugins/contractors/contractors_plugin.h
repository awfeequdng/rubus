#ifndef CONTRACTORS_PLUGIN_H
#define CONTRACTORS_PLUGIN_H

#include <QQmlExtensionPlugin>

class ContractorsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // CONTRACTORS_PLUGIN_H

