#include "initwindow.h"

InitWindow::InitWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InitWindow)
{
    ui->setupUi(this);

    //create window
    mainwindow = new MainWindow();
    addplayerwindow = new AddPlayerWindow();

    // Load the image files using the resource path:/resources/img/744px-Muskets.svg.png
    QPixmap owl(":/resources/img/Target owl.png");
    ui->label->setPixmap(owl);
}

InitWindow::~InitWindow(){
    delete ui;
    delete mainwindow;

    ui = nullptr;
    mainwindow = nullptr;
}

MainWindow* InitWindow::getMainWindow(){
    return mainwindow;
}

void InitWindow::on_exitButton_clicked(){
    this->close();
}

void InitWindow::on_continueButton_clicked(){
    this->hide();
    mainwindow->show();
}

void InitWindow::on_addButton_clicked(){
    addplayerwindow->show();
}





