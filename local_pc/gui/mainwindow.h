#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <ui_mainwindow.h>
#include <QDir>
#include <QCoreApplication>
#include <iostream>
#include "pistolwindow.h"
#include "riflewindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void openPistolWindowSignal();
    void openRifleWindowSignal();

public slots:
    PistolWindow *getPistolWindow();
    RifleWindow *getRifleWindow();

private slots:
    void on_exitButton_clicked();
    void openRifleWindow();
    void openPistolWindow();

private:
    Ui::MainWindow *ui;
    PistolWindow *pistolwindow = new PistolWindow();
    RifleWindow *riflewindow = new RifleWindow();
};
#endif // MAINWINDOW_H
