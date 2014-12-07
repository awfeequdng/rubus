#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtDeclarative/QDeclarativeProperty>

#include "core.h"
#include "user.h"

QString m_configFile;
QString m_user;
QString m_pwd;

void parseAppArgs()
{
    QStringList args = QApplication::arguments();
    args.removeFirst();

    if (args.isEmpty()) {
        return;
    }

    for (int i = 0; i < args.count();) {
        QString nextArg = i+1 < args.count() ? args.at(i + 1) : QString();

        if (args.at(i) == "-c" || args.at(i) == "--config") {
            if (nextArg.isEmpty() || nextArg.startsWith("-")) {
                qDebug() << "Invalid config file";
                i++;
                continue;
            }

            if (!QFile::exists(nextArg)) {
                qDebug() << nextArg << ": config file not found";
                i += 2;
                continue;
            }

            m_configFile = nextArg;
            i += 2;
        } else if (args.at(i) == "-u" || args.at(i) == "--user") {
            if (nextArg.isEmpty() || nextArg.startsWith("-")) {
                qDebug() << "Invalid user parameters";
                i++;
                continue;
            }

            m_user = nextArg;
            i += 2;
        } else if (args.at(i) == "-p" || args.at(i) == "--password") {
            if (nextArg.isEmpty() || nextArg.startsWith("-")) {
                qDebug() << "Invalid password parameters";
                i++;
                continue;
            }

            m_pwd = nextArg;
            i += 2;
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<Core::ICore>("Rubus", 1, 0, "Core");
    qmlRegisterType<Core::User>("Rubus", 1, 0, "User");

    QTranslator translator;
    translator.load("rubus_ru");
    app.installTranslator(&translator);

    parseAppArgs();

    Core::ICore core(m_configFile);


    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/qml/SigninDialog.qml"));

    QObject *signin = engine.rootObjects().first();
    signin->setProperty("username", m_user);
    signin->setProperty("password", m_pwd);

    return app.exec();
}
