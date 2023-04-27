#include "addplayerwindow.h"

AddPlayerWindow::AddPlayerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AddPlayerWindow)
{
    ui->setupUi(this);

}

AddPlayerWindow::~AddPlayerWindow(){
    delete ui;

    ui = nullptr;
}

void AddPlayerWindow::on_cancelButton_clicked(){
    this->close();
}

void AddPlayerWindow::on_saveButton_clicked(){
    this->close();
}
