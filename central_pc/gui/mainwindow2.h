#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QtWidgets/QMainWindow>
#include <ui_mainwindow2.h>
#include <QDir>
#include <QCoreApplication>
#include <QTableWidget>
#include <QWidget>
#include <QScreen>
#include "pistolwindow.h"
#include "riflewindow.h"

class MainWindow2 : public QMainWindow
{
    Q_OBJECT
signals:
    void backButtonClicked();

public:
    MainWindow2(QWidget *parent = 0);
    ~MainWindow2();
    bool rifle;
    bool pistol;  
    bool backSignal;

public slots:
    PistolWindow *getPistolWindow();
    RifleWindow *getRifleWindow();

private slots:
    void on_rifleButton_clicked();
    void on_pistolButton_clicked();
    void on_backButton_clicked();
    void on_exitButton_clicked();
    void onBackButtonClicked();

private:
    Ui::MainWindow2 *ui;
    PistolWindow *pistolwindow;
    RifleWindow *riflewindow;
};
#endif // MAINWINDOW2_H
