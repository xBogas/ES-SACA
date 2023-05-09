#include "initwindow.h"

InitWindow::InitWindow(QWidget *parent, Database *db)
    : QMainWindow(parent)
    , ui(new Ui::InitWindow)
    , database(db)
{
    ui->setupUi(this);

    ui->exitButton->setIcon(QIcon(":/resources/img/exit.png"));

    //create window
    mainwindow = new MainWindow();
    addplayerwindow = new AddPlayerWindow(nullptr, database);

    // Load the image files using the resource path:/resources/img/744px-Muskets.svg.png
    QPixmap owl(":/resources/img/Target owl.png");
    ui->label->setPixmap(owl);

    //conect signal
    connect(mainwindow, SIGNAL(backButtonClicked()), this, SLOT(onBackButtonClicked()));
}

InitWindow::~InitWindow(){
    delete ui;
    delete mainwindow;
    delete addplayerwindow;

    ui = nullptr;
    mainwindow = nullptr;
    addplayerwindow = nullptr;
}

MainWindow* InitWindow::getMainWindow(){
    return mainwindow;
}

void InitWindow::on_importButton_clicked(){
    
}

void InitWindow::on_exportButton_clicked(){
    
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
    addplayerwindow->move(100,100);
}

void InitWindow::onBackButtonClicked(){
    this->show();
}







