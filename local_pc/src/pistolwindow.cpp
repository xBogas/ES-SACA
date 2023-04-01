#include "pistolwindow.h"
#include "ui_pistolwindow.h"

PistolWindow::PistolWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PistolWindow)
{
    ui->setupUi(this);

    QPixmap PistolTarget(":/resources/img/PistolTarget.png");
    ui->Target->setPixmap(PistolTarget);
    ui->ExitButton->setIcon(QIcon(":/resources/img/exit.png"));


    segundos=0;
    minutos=0;
    horas=0;
    procss=0;
    connect(&reloj,SIGNAL(timeout()),this,SLOT(processar()));
}

PistolWindow::~PistolWindow()
{
    delete ui;
}

void PistolWindow::on_StartButton_clicked()
{  
    reloj.start(1000);
    procss=1;
}

void PistolWindow::on_PracticeButton_clicked()
{  
    if(procss==0){
        segundos=0;
        minutos=15;
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