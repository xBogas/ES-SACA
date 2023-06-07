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
    QPixmap rfl(":/resources/img/744px-Muskets.svg.png");
    ui->rifleLabel->setPixmap(rfl);
    QPixmap ptl(":/resources/img/crossed-pistols-on-the-black-background.gif");
    ui->pistolLabel->setPixmap(ptl);
    ui->exitButton->setIcon(QIcon(":/resources/img/exit.png"));
    
    pistolwindow = new PistolWindow();
    riflewindow = new RifleWindow();

    rifle = false;
    pistol = false; 
    backSignal = false;

    //conect signal
    connect(pistolwindow, SIGNAL(backButtonClicked()), this, SLOT(onBackButtonClicked()));
    connect(riflewindow, SIGNAL(backButtonClicked()), this, SLOT(onBackButtonClicked()));
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
    riflewindow->showFullScreen();

    pistol = false;
    rifle = true;
}

void MainWindow2::on_pistolButton_clicked(){
    this->hide();
    pistolwindow->showFullScreen();

    rifle = false;
    pistol = true;
}

void MainWindow2::on_exitButton_clicked(){
    this->close();
}

void MainWindow2::on_backButton_clicked(){
    this->hide();
    backSignal = true;
    emit backButtonClicked();
}

void MainWindow2::onBackButtonClicked(){
    this->show();
    pistol = false;
    rifle = false;
}


