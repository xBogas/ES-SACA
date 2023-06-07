#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <ui_mainwindow.h>
#include <QDir>
#include <QCoreApplication>
#include <QTableWidget>
#include <QWidget>
#include "mainwindow2.h"
#include <QMessageBox>
#include <iostream>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool isMainWindow = true;
    std::map<std::string, int> clientPlayerIds;
    std::map<std::string, bool> nonPlayerIds;
    std::map<std::string, bool> samePlayerIds;
    std::map<std::string, bool> cellWasChanged;
    std::string nome;
    std::string local;
    std::string data;

signals:
    void showErrorMessageSignal(std::string errorType);
    void backButtonClicked();

public slots:
    void updateClientList(std::vector<std::string> clients);
    MainWindow2* getMainWindow2();

private slots:
    void on_exitButton_clicked();
    void on_continueButton_clicked();
    void on_IP_ID_Table_itemChanged(QTableWidgetItem* item);
    void on_backButton_clicked();
    void onBackButtonClicked();
    void showErrorMessage(std::string errorType);
    bool differentID(int ID, int row);
    bool rightIDs();
    void updateCellWasChanged(std::string clientIp);
    
    
private:
    std::vector<std::string> connected_clients;
    Ui::MainWindow *ui;
    MainWindow2 *mainwindow2;
    QTableWidget *IP_ID_Table;
};
#endif // MAINWINDOW_H
