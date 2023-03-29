#include "pistolwindow.h"
#include "ui_pistolwindow.h"

PistolWindow::PistolWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PistolWindow)
{
    ui->setupUi(this);


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
    }
    if(minutos==0 && segundos==0) {
        minutos=60;
        horas=horas-1;
    }
    if(segundos==0 && minutos!=0) {
        segundos=60;
        minutos=minutos-1;
    }

}