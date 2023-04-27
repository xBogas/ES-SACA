#include "addplayerwindow.h"

AddPlayerWindow::AddPlayerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AddPlayerWindow)
{
    ui->setupUi(this);

    // Load the image files using the resource path:/resources/img/744px-Muskets.svg.png
    QPixmap owl(":/resources/img/Target owl.png");
    ui->label->setPixmap(owl);
}

AddPlayerWindow::~AddPlayerWindow(){
    delete ui;

    ui = nullptr;
}

void AddPlayerWindow::on_cancelButton_clicked(){
    this->close();
}

void AddPlayerWindow::on_saveButton_clicked(){
    this->hide();
}





