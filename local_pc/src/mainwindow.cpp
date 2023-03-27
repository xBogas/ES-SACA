#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load the image files using the resource path:/resources/img/744px-Muskets.svg.png
    QPixmap owl(":/resources/img/Target owl.png");
    ui->label->setPixmap(owl);

    QPixmap impist(":/resources/img/crossed-pistols-on-the-black-background.gif");
    ui->label_2->setPixmap(impist);

    QPixmap imrif(":/resources/img/744px-Muskets.svg.png");
    ui->label_3->setPixmap(imrif);
}

MainWindow::~MainWindow()
{
    delete ui;
}

