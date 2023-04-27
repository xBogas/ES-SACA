#ifndef INITWINDOW_H
#define INITWINDOW_H

#include <QtWidgets/QMainWindow>
#include <ui_initwindow.h>
#include <QDir>
#include <QCoreApplication>
#include <QTableWidget>
#include <QWidget>
#include "mainwindow.h"
#include "addplayerwindow.h"
#include <QMessageBox>

class InitWindow : public QMainWindow
{
    Q_OBJECT

public:
    InitWindow(QWidget *parent = 0);
    ~InitWindow();

private:
    

signals:

public slots:
    MainWindow* getMainWindow();

private slots:
    void on_exitButton_clicked();
    void on_continueButton_clicked();
    void on_addButton_clicked();
    
private:
    Ui::InitWindow *ui;
    MainWindow *mainwindow;
    AddPlayerWindow *addplayerwindow;
};
#endif // INITWINDOW_H
