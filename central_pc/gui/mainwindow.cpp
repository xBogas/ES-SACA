#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //create mainwindow2
    mainwindow2 = new MainWindow2();

    // Load the image files using the resource path:/resources/img/744px-Muskets.svg.png
    QPixmap owl(":/resources/img/Target owl.png");
    ui->label->setPixmap(owl);

    //tablewidgetconnect 94.130.134.104:27023
    //IP_ID_Table = findChild<QTableWidget*>("IP_ID_Table");
    ui->IP_ID_Table->setHorizontalHeaderLabels({"Endereço IP", "ID do Atleta"});
    ui->IP_ID_Table->setColumnWidth(0, 245);
    ui->IP_ID_Table->setColumnWidth(1, 135);

    //conect signals
    QObject::connect(this, &MainWindow::showErrorMessageSignal, this, &MainWindow::showErrorMessage);
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
    if(rightIDs()){
        isMainWindow = false;
        this->hide();
        mainwindow2->show();
    }
}

bool MainWindow::rightIDs(){
    for (const auto& client : connected_clients){
        if(nonPlayerIds[client] || samePlayerIds[client])
            return false;
    }
    
    // for(int row = 0; row < ui->IP_ID_Table->rowCount(); row++){
    //     QTableWidgetItem *item = ui->IP_ID_Table->item(row, 1);
    //     if(item != nullptr){
    //         int value = item->text().toInt();

    //         for(int rowAux = 0; rowAux < ui->IP_ID_Table->rowCount(); rowAux++){
    //             if(row != rowAux){
    //                 QTableWidgetItem *itemAux = ui->IP_ID_Table->item(rowAux, 1);
    //                 if(itemAux != nullptr){
    //                     int valueAux = itemAux->text().toInt();

    //                     if(value == valueAux)
    //                         return false;
    //                 }
    //             }
    //         }
    //     }
        
    // }

    return true;
}

void MainWindow::updateClientList(std::vector<std::string> clients){
    ui->IP_ID_Table->setRowCount(clients.size());
    int row = 0;

    connected_clients = clients;
    
    for (const auto& client : clients){
        QTableWidgetItem *ipItem = new QTableWidgetItem(QString::fromStdString(client));
        ipItem->setFlags(ipItem->flags() ^ Qt::ItemIsEditable);
        ui->IP_ID_Table->setItem(row, 0, ipItem);

        // QTableWidgetItem *playerIdItem = new QTableWidgetItem(QString::number(clientPlayerIds[client]));
        // ui->IP_ID_Table->setItem(row, 1, playerIdItem);

        row++;
    }
}

void MainWindow::on_IP_ID_Table_cellChanged(int row, int column){
    if (column == 1){
        std::string clientIp = ui->IP_ID_Table->item(row, 0)->text().toStdString();
        int playerId = ui->IP_ID_Table->item(row, 1)->text().toInt();

        if(differentID(playerId, row)){
            clientPlayerIds[clientIp] = playerId;
            samePlayerIds[clientIp] = false;
        }
        else    
            samePlayerIds[clientIp] = true;

        updateCellWasChanged(clientIp);
    }
}

bool MainWindow::differentID(int ID, int row){
    for(int rowNumber = 0; rowNumber < ui->IP_ID_Table->rowCount(); rowNumber++) {
        if(rowNumber != row){
            QTableWidgetItem *item = ui->IP_ID_Table->item(rowNumber, 1);
            if(item != nullptr){
                int value = item->text().toInt();

                if(ID == value){
                    return false;
                }
            }
        }
    }

    return true;
}

void MainWindow::updateCellWasChanged(std::string clientIp){
    for (const auto& client : connected_clients){
        if(client == clientIp)
            cellWasChanged[clientIp] = true;
        else    
            cellWasChanged[clientIp] = false;
    }
}

void MainWindow::showErrorMessage(std::string errorType){
    if(errorType == "nonID")
        QMessageBox::critical(this, "Erro", "ID inexistente");
    else if(errorType == "sameID")
        QMessageBox::critical(this, "Erro", "ID já atribuído");
}



