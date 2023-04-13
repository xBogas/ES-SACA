#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QCoreApplication>
#include <QTableWidget>
#include <QWidget>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load the image files using the resource path:/resources/img/744px-Muskets.svg.png
    QPixmap owl(":/resources/img/Target owl.png");
    ui->label->setPixmap(owl);

    //tablewidget
    IP_ID_Table = findChild<QTableWidget*>("IP_ID_Table");
    IP_ID_Table->setHorizontalHeaderLabels({"IP Address", "Player ID"});
    IP_ID_Table->setColumnWidth(0, 245);
    IP_ID_Table->setColumnWidth(1, 135);
}

MainWindow::~MainWindow(){
    delete ui;
    delete mainwindow2;

    ui = nullptr;
    mainwindow2 = nullptr;
}

MainWindow2* MainWindow::getMainWindow2(){
    return mainwindow2;
}

void MainWindow::on_exitButton_clicked(){
    this->close();
}

void MainWindow::on_continueButton_clicked(){
    bool allNumbersGreaterThanZero = true;
    for(int row = 0; row < IP_ID_Table->rowCount(); row++) {
        QTableWidgetItem *item = IP_ID_Table->item(row, 1);
        int value = item->text().toInt();
        if(value <= 0) {
            allNumbersGreaterThanZero = false;
            break;
        }
    }
    if(allNumbersGreaterThanZero) {
        isMainWindow = false;
        this->hide();
        mainwindow2->show();
    }
}

void MainWindow::updateClientList(std::vector<std::string> clients){
    IP_ID_Table->setRowCount(clients.size());
    int row = 0;
    
    for (const auto& client : clients){
        QTableWidgetItem *ipItem = new QTableWidgetItem(QString::fromStdString(client));
        ipItem->setFlags(ipItem->flags() ^ Qt::ItemIsEditable);
        IP_ID_Table->setItem(row, 0, ipItem);

        QTableWidgetItem *playerIdItem = new QTableWidgetItem(QString::number(clientPlayerIds[client]));
        IP_ID_Table->setItem(row, 1, playerIdItem);

        row++;
    }
}

void MainWindow::on_IP_ID_Table_cellChanged(int row, int column){
    if (column == 1){
        std::string clientIp = IP_ID_Table->item(row, 0)->text().toStdString();
        int playerId = IP_ID_Table->item(row, 1)->text().toInt();
        clientPlayerIds[clientIp] = playerId;

        cellWasChanged = true;
    }
}
