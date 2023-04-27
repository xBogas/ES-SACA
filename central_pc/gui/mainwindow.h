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

private:
    std::vector<std::string> connected_clients;

signals:
    void showErrorMessageSignal(std::string errorType);

public slots:
    void updateClientList(std::vector<std::string> clients);
    MainWindow2* getMainWindow2();

private slots:
    void on_exitButton_clicked();
    void on_continueButton_clicked();
    void on_IP_ID_Table_cellChanged(int row, int column);
    void showErrorMessage(std::string errorType);
    bool differentID(int ID, int row);
    bool rightIDs();
    void updateCellWasChanged(std::string clientIp);
    
private:
    Ui::MainWindow *ui;
    MainWindow2 *mainwindow2;
    QTableWidget *IP_ID_Table;
};
#endif // MAINWINDOW_H
