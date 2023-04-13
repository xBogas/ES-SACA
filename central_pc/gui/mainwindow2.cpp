#include "mainwindow2.h"
#include "ui_mainwindow2.h"
#include <QDir>
#include <QCoreApplication>
#include <QTableWidget>
#include <QWidget>

MainWindow2::MainWindow2(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow2)
{
    ui->setupUi(this);

    // Load the image files using the resource path:/resources/img/744px-Muskets.svg.png
    QPixmap owl(":/resources/img/Target owl.png");
    ui->label->setPixmap(owl);
}

MainWindow2::~MainWindow2(){
    delete ui;
}

PistolWindow* MainWindow2::getPistolWindow(){
    return pistolwindow;
}

RifleWindow* MainWindow2::getRifleWindow(){
    return riflewindow;
}

void MainWindow2::on_rifleButton_clicked(){
    this->hide();
    riflewindow->show();

    pistol = false;
    rifle = true;
}

void MainWindow2::on_pistolButton_clicked(){
    this->hide();
    pistolwindow->show();

    rifle = false;
    pistol = true;
}


