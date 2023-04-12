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

public slots:
    void updateClientList(std::vector<std::string> clients);

private slots:
    void on_exitButton_clicked();
    void on_continueButton_clicked();
    void onPlayerIdChanged(int row, int column);

private:
    Ui::MainWindow *ui;
    MainWindow2 *mainwindow2;
    QTableWidget *IP_ID_Table;
    std::map<std::string, int> clientPlayerIds;
};
#endif // MAINWINDOW_H
