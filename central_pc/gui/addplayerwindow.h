#ifndef ADDPLAYERWINDOW_H
#define ADDPLAYERWINDOW_H

#include <QtWidgets/QMainWindow>
#include <ui_addplayerwindow.h>
#include <QDir>
#include <QCoreApplication>
#include <QTableWidget>
#include <QWidget>
#include <QMessageBox>
#include <iostream>
#include "../database/database.h"

class AddPlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    AddPlayerWindow(QWidget *parent = 0, Database *db = nullptr);
    ~AddPlayerWindow();

private:
    Database* database;
    
signals:

public slots:

private slots:
    void on_cancelButton_clicked();
    void on_saveButton_clicked();
    
private:
    Ui::AddPlayerWindow *ui;
};
#endif // ADDPLAYERWINDOW_H
