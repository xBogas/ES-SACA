#ifndef EXPORTWINDOW_H
#define EXPORTWINDOW_H

#include <QtWidgets/QMainWindow>
#include <ui_exportwindow.h>
#include <QDir>
#include <QCoreApplication>
#include <QTableWidget>
#include <QWidget>
#include <QMessageBox>
#include <QDate>
#include <iostream>
#include "../database/database.h"

class ExportWindow : public QMainWindow
{
    Q_OBJECT

public:
    ExportWindow(QWidget *parent = 0, Database *db = nullptr);
    ~ExportWindow();

    void fillTextIfExist();

    std::string data;
    std::string categoria;
    std::string local;

private:
    Database* database;
    
signals:

public slots:

private slots:
    void on_cancelButton_clicked();
    void on_saveButton_clicked();
    
private:
    Ui::ExportWindow *ui;
};
#endif // EXPORTWINDOW_H
