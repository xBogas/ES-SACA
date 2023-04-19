#include "pistolwindow.h"
#include "ui_pistolwindow.h"
#include <QGraphicsPixmapItem>

PistolWindow::PistolWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PistolWindow)
{
    ui->setupUi(this);

    int w = ui->Target->width();
    int h = ui->Target->height();
    QPixmap PistolTarget(":/resources/img/PistolTarget.png");
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addPixmap(PistolTarget.scaled(w-2,h-2));
    ui->Target->setScene(scene);

    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
    item->setScale(0.01);
    item->setPos(283.5,249);
    scene->addItem(item);
    
    QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
    item2->setScale(0.01);
    item2->setPos(300,240);
    scene->addItem(item2);
    

    ui->ExitButton->setIcon(QIcon(":/resources/img/exit.png"));
    
    practiceSignal = false;
    matchSignal = false;
    finalSignal = false;
    startSignal = false;

    segundos=0;
    minutos=0;
    horas=0;
    procss=0;
    al=0;

    connect(&reloj,SIGNAL(timeout()),this,SLOT(processar()));
    connect(&alert,SIGNAL(timeout()),this,SLOT(alerta()));
}

PistolWindow::~PistolWindow()
{
    delete ui;
}

void PistolWindow::on_StartButton_clicked()
{  
    startSignal = true;

    reloj.start(1000);
    alert.start(250);
    procss=1;
}

void PistolWindow::on_PracticeButton_clicked()
{ 
    if(procss==0){
        practiceSignal = true;
        matchSignal = false;
        finalSignal = false;

        segundos=2;
        minutos=1;
        horas=0;
        ui->seconds->display(segundos);
        ui->minutes->display(minutos);
        ui->hours->display(horas);
        ui->StartButton->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0)}");
        ui->PracticeButton->setStyleSheet("QPushButton{background-color: rgb(255, 255, 0)}");
        ui->MatchButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
        ui->FinalButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
    }
}

void PistolWindow::on_MatchButton_clicked()
{  
    if(procss==0){
        matchSignal = true;
        practiceSignal = false;
        finalSignal = false;

        segundos=0;
        minutos=15;
        horas=1;
        ui->seconds->display(segundos);
        ui->minutes->display(minutos);
        ui->hours->display(horas);
        ui->StartButton->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0)}");
        ui->PracticeButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
        ui->MatchButton->setStyleSheet("QPushButton{background-color: rgb(170, 0, 0)}");
        ui->FinalButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
    }
}

void PistolWindow::on_FinalButton_clicked()
{ 
    if(procss==0){
        finalSignal = true;
        practiceSignal = false;
        matchSignal = false;

        segundos=0;
        minutos=15;
        horas=1;
        ui->seconds->display(segundos);
        ui->minutes->display(minutos);
        ui->hours->display(horas);
        ui->StartButton->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0)}");
        ui->PracticeButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
        ui->MatchButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
        ui->FinalButton->setStyleSheet("QPushButton{background-color: rgb(85, 85, 255)}");
    }
}

void PistolWindow::on_MainButton_clicked(){
    this->hide();
    this->parentWidget()->show();
}

void PistolWindow::on_ExitButton_clicked()
{
    qApp->quit();
}

void PistolWindow::on_horizontalSlider_valueChanged(int value){
    int w = ui->Target->width();
    int h = ui->Target->height();
    ui->Target->setTransform(QTransform::fromScale(value/100,value/100));
}

void PistolWindow::processar()
{
    if(minutos==0 && segundos==0) {
        minutos=60;
        horas=horas-1;
    }
    if(segundos==0 && minutos!=0) {
        segundos=60;
        minutos=minutos-1;
    }
    segundos=segundos-1; // segundos--;
    ui->seconds->display(segundos);
    ui->minutes->display(minutos);
    ui->hours->display(horas);
    if(segundos==0 && minutos==0 && horas==0){
        reloj.stop();
        procss=0;
        ui->StartButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
        ui->PracticeButton->setStyleSheet("QPushButton{background-color: rgb(255, 255, 0)}");
        ui->MatchButton->setStyleSheet("QPushButton{background-color: rgb(170, 0, 0)}");
        ui->FinalButton->setStyleSheet("QPushButton{background-color: rgb(85, 85, 255)}");
    }
    if(minutos==0 && segundos==0) {
        minutos=60;
        horas=horas-1;
    }
    if(segundos==0 && minutos!=0) {
        segundos=60;
        minutos=minutos-1;
    }

    // Butão Start intermitente
    if(procss==1 && segundos%2==0){
        ui->StartButton->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0)}");
    }
    if(procss==1 && segundos%2==1){
        ui->StartButton->setStyleSheet("QPushButton{background-color: rgb(85, 255, 0)}");
    }
}

void PistolWindow::alerta()
{
    if(al==0){
        // Aviso de tempo a acabar
        if(procss==1 && (horas==0 && minutos==1 && segundos==0 || horas==0 && minutos==0 && segundos>>0)){
            ui->seconds->setStyleSheet("QLCDNumber{color: rgb(250, 0, 0)}");
            ui->minutes->setStyleSheet("QLCDNumber{color: rgb(250, 0, 0)}");
            ui->hours->setStyleSheet("QLCDNumber{color: rgb(250, 0, 0)}");
        }
        al=1;
    }
    else{
        // Aviso de tempo a acabar
        if(procss==1 && (horas==0 && minutos==1 && segundos==0 || horas==0 && minutos==0 && segundos>>0)){
            ui->seconds->setStyleSheet("QLCDNumber{color: rgb(0, 0, 0)}");
            ui->minutes->setStyleSheet("QLCDNumber{color: rgb(0, 0, 0)}");
            ui->hours->setStyleSheet("QLCDNumber{color: rgb(0, 0, 0)}");
        }
        al=0;
    }
    if(procss==0){
        alert.stop();
        ui->seconds->setStyleSheet("QLCDNumber{color: rgb(0, 0, 0)}");
        ui->minutes->setStyleSheet("QLCDNumber{color: rgb(0, 0, 0)}");
        ui->hours->setStyleSheet("QLCDNumber{color: rgb(0, 0, 0)}");
    }
}
