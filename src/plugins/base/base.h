#ifndef BASE_H
#define BASE_H

#include "base_global.h"
#include "iplugin.h"

class QAction;

class BASE_EXPORT Base : public Core::IPlugin
{
    Q_OBJECT
    Q_INTERFACES(Core::IPlugin)

#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "Base")
#endif

public:
    explicit Base(QObject *parent = 0);

    static Base *instance();

    QString name() const;
    int version() const;
    bool initialize();

public slots:
    static void showUserManager();
    static void showReportManager();
    static void showLocationTable();
    static void showContractorTable();
    static void showItemTable();


private:
    QAction *m_acUserManager;
    QAction *m_acReportManager;
    QAction *m_acLocations;
    QAction *m_acContractors;
    QAction *m_acItems;
};

#endif // BASE_H
