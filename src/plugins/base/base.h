#ifndef BASE_H
#define BASE_H

#include "base_global.h"
#include "iplugin.h"

class BASE_EXPORT Base : public Core::IPlugin
{

public:
    explicit Base(QObject *parent = 0);

    QString name() const;
    int version() const;
    bool initialize();
};

#endif // BASE_H
