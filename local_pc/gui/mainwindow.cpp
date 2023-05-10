#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load the image files using the resource path:/resources/img/744px-Muskets.svg.png
    QPixmap owl(":/resources/img/Target owl.png");
    ui->label->setPixmap(owl);

    pistolwindow = new PistolWindow();
    riflewindow = new RifleWindow();

    //conect signals
    QObject::connect(this, &MainWindow::openPistolWindowSignal, this, &MainWindow::openPistolWindow);
    QObject::connect(this, &MainWindow::openRifleWindowSignal, this, &MainWindow::openRifleWindow);
    connect(pistolwindow, SIGNAL(backButtonClickedSignal()), this, SLOT(onBackButtonClicked()));
    connect(riflewindow, SIGNAL(backButtonClickedSignal()), this, SLOT(onBackButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

PistolWindow* MainWindow::getPistolWindow(){
    return pistolwindow;
}

RifleWindow* MainWindow::getRifleWindow(){
    return riflewindow;
}

void MainWindow::openRifleWindow()
{
    this->hide();
    riflewindow->show();
}

void MainWindow::openPistolWindow()
{
    this->hide();
    pistolwindow->show();
}

void MainWindow::on_exitButton_clicked()
{
    qApp->quit();
}

void MainWindow::onBackButtonClicked(){
    this->show();
}