#ifndef %PluginName:u%_H
#define %PluginName:u%_H

#include "%ProjectName:l%_global.%CppHeaderSuffix%"
#include "iplugin.h"

class %ProjectName:u%_EXPORT %PluginName% : public Core::IPlugin
{
    Q_OBJECT
    Q_INTERFACES(Core::IPlugin)
#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "%PluginName%")
#endif

public:
    explicit %PluginName%(QObject *parent = 0);
    ~%PluginName%();

    static %PluginName% *instance();

    QString name() const;
    int version() const;
    bool initialize();

public slots:

private:
};

#endif // %PluginName:u%_H
