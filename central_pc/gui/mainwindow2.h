#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QtWidgets/QMainWindow>
#include <ui_mainwindow2.h>
#include <QDir>
#include <QCoreApplication>
#include <QTableWidget>
#include <QWidget>
#include "pistolwindow.h"
#include "riflewindow.h"

class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow2(QWidget *parent = 0);
    ~MainWindow2();
    bool rifle = false;
    bool pistol = false;  

public slots:
    PistolWindow *getPistolWindow();
    RifleWindow *getRifleWindow();

private slots:
    void on_rifleButton_clicked();
    void on_pistolButton_clicked();

private:
    Ui::MainWindow2 *ui;
    PistolWindow *pistolwindow = new PistolWindow();
    RifleWindow *riflewindow = new RifleWindow();
};
#endif // MAINWINDOW2_H
