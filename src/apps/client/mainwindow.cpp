#include "mainwindow.h"
#include <QQmlApplicationEngine>

MainWindow::MainWindow(QQmlApplicationEngine *qmlEngine, QObject *parent) :
    QObject(parent)
{
    m_qmlEngine = qmlEngine;
    connect(Core::ICore::instance(), SIGNAL(logged()), SLOT(onCoreLogged()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::onCoreLogged()
{
    m_qmlEngine->load(QUrl("qrc:/qml/MainWindow.qml"));
}

