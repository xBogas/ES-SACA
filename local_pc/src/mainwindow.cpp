#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap owl("local_pc/resource/img/744px-Muskets.svg.png");
    ui->label->setPixmap(owl);
    QPixmap impist("/home/cassienc/ES-S/ES-SACA/local_pc/resource/img/crossed-pistols-on-the-black-background.gif");
    ui->label_2->setPixmap(impist);
    QPixmap imrif("/home/cassienc/ES-S/ES-SACA/local_pc/resource/img/744px-Muskets.svg.png");
    ui->label_3->setPixmap(imrif);
}

MainWindow::~MainWindow()
{
    delete ui;
}

