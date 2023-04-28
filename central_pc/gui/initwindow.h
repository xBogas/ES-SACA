#ifndef INITWINDOW_H
#define INITWINDOW_H

#include <QtWidgets/QMainWindow>
#include <ui_initwindow.h>
#include <QDir>
#include <QCoreApplication>
#include <QTableWidget>
#include <QWidget>
#include <QMessageBox>
#include "mainwindow.h"
#include "addplayerwindow.h"
#include "../database/database.h"

class InitWindow : public QMainWindow
{
    Q_OBJECT

public:
    InitWindow(QWidget *parent = 0, Database *db = nullptr);
    ~InitWindow();
    
signals:

public slots:
    MainWindow* getMainWindow();

private slots:
    void on_exitButton_clicked();
    void on_continueButton_clicked();
    void on_addButton_clicked();
    void on_importButton_clicked();
    void on_exportButton_clicked();
    
private:
    Ui::InitWindow *ui;
    MainWindow *mainwindow;
    AddPlayerWindow *addplayerwindow;
    Database *database;
};
#endif // INITWINDOW_H
