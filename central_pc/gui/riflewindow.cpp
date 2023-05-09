#include "riflewindow.h"
#include "ui_riflewindow.h"

RifleWindow::RifleWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RifleWindow)
{
    ui->setupUi(this);

    QPixmap RifleTarget(":/resources/img/RifleTarget.png");
    ui->Target->setPixmap(RifleTarget);
    ui->ExitButton->setIcon(QIcon(":/resources/img/exit.png"));

    practiceSignal = false;
    matchSignal = false;
    finalSignal = false;
    startSignal = false;
    switchModeSignal = false;
    backSignal = false;
    block = false;

    segundos=0;
    minutos=0;
    horas=0;
    procss=0;
    al=0;

    connect(&reloj,SIGNAL(timeout()),this,SLOT(processar()));
    connect(&alert,SIGNAL(timeout()),this,SLOT(alerta()));
    QObject::connect(this, &RifleWindow::backToDecideModeSignal, this, &RifleWindow::resetTimer);
}

RifleWindow::~RifleWindow()
{
    delete ui;
}

void RifleWindow::on_StartButton_clicked()
{  
    if(!block){
        startSignal = true;

        reloj.start(1000);
        alert.start(250);
        procss=1;
    }
}

void RifleWindow::on_PracticeButton_clicked()
{  
    if(procss==0 && !block){
        practiceSignal = true;
        matchSignal = false;
        finalSignal = false;

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

void RifleWindow::on_MatchButton_clicked()
{  
    if(procss==0 && !block){
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

void RifleWindow::on_FinalButton_clicked()
{ 
    if(procss==0 && !block){
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

void RifleWindow::on_ExitButton_clicked()
{
    qApp->quit();
}

void RifleWindow::on_switchButton_clicked()
{
    switchModeSignal = true;
    blockDecideMode();
}

void RifleWindow::on_backButton_clicked()
{
    backSignal = true;

    this->close();
    emit backButtonClicked();
}

void RifleWindow::on_horizontalSlider_valueChanged(int value){
    QPixmap RifleTarget(":/resources/img/RifleTarget.png");
    int w = ui->Target->width();
    int h = ui->Target->height();
    if(value==0){
        ui->Target->setScaledContents(true);
        ui->Target->setPixmap(RifleTarget);
    }
    else{
        ui->Target->setScaledContents(false);
        ui->Target->setAlignment(Qt::AlignCenter);
        ui->Target->setPixmap(RifleTarget.scaled(w+value,h+value));
    }
}

void RifleWindow::processar()
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
        resetTimer();
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

void RifleWindow::alerta()
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

void RifleWindow::resetTimer()
{  
    reloj.stop();
    procss = 0;
    segundos = 0;
    minutos = 0;
    horas = 0;
    block = false;
    ui->StartButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
    ui->PracticeButton->setStyleSheet("QPushButton{background-color: rgb(255, 255, 0)}");
    ui->MatchButton->setStyleSheet("QPushButton{background-color: rgb(170, 0, 0)}");
    ui->FinalButton->setStyleSheet("QPushButton{background-color: rgb(85, 85, 255)}");
}

void RifleWindow::blockDecideMode(){
    reloj.stop();
    procss = 0;
    segundos = 0;
    minutos = 0;
    horas = 0;
    block = true;
    ui->StartButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
    ui->PracticeButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
    ui->MatchButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
    ui->FinalButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
}