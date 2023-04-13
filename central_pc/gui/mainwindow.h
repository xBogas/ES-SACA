#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <ui_mainwindow.h>
#include <QDir>
#include <QCoreApplication>
#include <QTableWidget>
#include <QWidget>
#include "mainwindow2.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool isMainWindow = true;
    bool cellWasChanged = false;
    std::map<std::string, int> clientPlayerIds;

public slots:
    void updateClientList(std::vector<std::string> clients);
    MainWindow2* getMainWindow2();

private slots:
    void on_exitButton_clicked();
    void on_continueButton_clicked();
    void on_IP_ID_Table_cellChanged(int row, int column);

private:
    Ui::MainWindow *ui;
    MainWindow2 *mainwindow2 = new MainWindow2();
    QTableWidget *IP_ID_Table;
};
#endif // MAINWINDOW_H
