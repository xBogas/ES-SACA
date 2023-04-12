#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QtWidgets/QMainWindow>
#include <ui_mainwindow2.h>
#include <QDir>
#include <QCoreApplication>
#include <QTableWidget>
#include <QWidget>
//#include "mainwindow3.h"

class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow2(QWidget *parent = 0);
    ~MainWindow2();

public slots:

private slots:

private:
    Ui::MainWindow2 *ui;
    //MainWindow3 *mainwindow3;
};
#endif // MAINWINDOW2_H
