#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>

#include "core.h"

class QQmlApplicationEngine;

class MainWindow : public QObject
{
    Q_OBJECT
public:
    explicit MainWindow(QQmlApplicationEngine *qmlEngine, QObject *parent = 0);
    ~MainWindow();

signals:

private slots:
    void onCoreLogged();

private:
    QQmlApplicationEngine *m_qmlEngine;

};

#endif // MAINWINDOW_H
