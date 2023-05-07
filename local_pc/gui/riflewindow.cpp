#include "riflewindow.h"
#include "ui_riflewindow.h"
#include <QGraphicsPixmapItem>

RifleWindow::RifleWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RifleWindow)
{
    ui->setupUi(this);

    w = ui->Target->width();
    h = ui->Target->height();
    QPixmap PistolTarget(":/resources/img/RifleTarget.png");
    scene = new QGraphicsScene(this);
    scene->addPixmap(PistolTarget.scaled(w,h));
    ui->Target->setScene(scene);

    ui->ExitButton->setIcon(QIcon(":/resources/img/exit.png"));

    QStringList titulos; 
    ui->tableWidget->setColumnCount(3);
    titulos << "Tiro" << "Pont.(Int)" << "Pont.(Dec)";
    ui->tableWidget->setHorizontalHeaderLabels(titulos);
    ui->tableWidget->verticalHeader()->setVisible(0);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    segundos=0;
    minutos=0;
    horas=0;
    procss=0;
    al=0;
    nim=1;
    zoom=0;
    timezoom=0;
    intshot=0;
    totalintshot=0;
    decshot=0;
    totaldecshot=0;

    connect(&reloj,SIGNAL(timeout()),this,SLOT(processar()));
    connect(&alert,SIGNAL(timeout()),this,SLOT(alerta()));
    connect(&timedzoom,SIGNAL(timeout()),this,SLOT(shootzoom()));

    QObject::connect(this, &RifleWindow::finalButtonClickedSignal, this, &RifleWindow::finalButtonClicked);
    QObject::connect(this, &RifleWindow::practiceButtonClickedSignal, this, &RifleWindow::practiceButtonClicked);
    QObject::connect(this, &RifleWindow::matchButtonClickedSignal, this, &RifleWindow::matchButtonClicked);
    QObject::connect(this, &RifleWindow::startButtonClickedSignal, this, &RifleWindow::startButtonClicked);
}

RifleWindow::~RifleWindow()
{
    delete ui;
}

void RifleWindow::startButtonClicked()
{  
   reloj.start(1000);
   alert.start(250);
   procss=1;
}

//Alterar os botões pelos que vão ser usados no pc _central, adicionar um include ao .h do ui onde os botões se encontram.
void RifleWindow::practiceButtonClicked()
{  
   if(procss==0){
       segundos=0;
       minutos=15;
       horas=0;
       ui->seconds->display(segundos);
       ui->minutes->display(minutos);
       ui->hours->display(horas);
       ui->Start->setStyleSheet("QLabel{background-color: rgb(85, 170, 0)}");
       ui->Practice->setStyleSheet("QLabel{background-color: rgb(255, 255, 0)}");
       ui->Match->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
       ui->Final->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
   }
}

void RifleWindow::matchButtonClicked()
{  
   if(procss==0){
       segundos=0;
       minutos=15;
       horas=1;
       ui->seconds->display(segundos);
       ui->minutes->display(minutos);
       ui->hours->display(horas);
       ui->Start->setStyleSheet("QLabel{background-color: rgb(85, 170, 0)}");
       ui->Practice->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
       ui->Match->setStyleSheet("QLabel{background-color: rgb(170, 0, 0)}");
       ui->Final->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
   }
}

void RifleWindow::finalButtonClicked()
{ 
   if(procss==0){
       segundos=0;
       minutos=15;
       horas=1;
       ui->seconds->display(segundos);
       ui->minutes->display(minutos);
       ui->hours->display(horas);
       ui->Start->setStyleSheet("QLabel{background-color: rgb(85, 170, 0)}");
       ui->Practice->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
       ui->Match->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
       ui->Final->setStyleSheet("QLabel{background-color: rgb(85, 85, 255)}");
   }
}

void RifleWindow::on_MainButton_clicked(){
    this->hide();
    this->parentWidget()->show();
}

void RifleWindow::on_ExitButton_clicked()
{
    qApp->quit();
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
        reloj.stop();
        procss=0;
        ui->Start->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
        ui->Practice->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
        ui->Match->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
        ui->Final->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
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
        ui->Start->setStyleSheet("QLabel{background-color: rgb(85, 170, 0)}");
    }
    if(procss==1 && segundos%2==1){
        ui->Start->setStyleSheet("QLabel{background-color: rgb(85, 255, 0)}");
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

void RifleWindow::showEvent(QShowEvent *revent){
    ui->Target->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}

void RifleWindow::resizeEvent(QResizeEvent *event){
    if(zoom==0){
        ui->Target->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    }
    else{
        ui->Target->centerOn(QPointF(259,259));
    }
    QMainWindow::resizeEvent(event);
}

void RifleWindow::on_horizontalSlider_valueChanged(int value){
    ui->Target->setTransform(QTransform::fromScale(value/100,value/100));
    if(value/100==1){
        ui->Target->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
        zoom=0;
    }
    else{
        zoom=1;
    }
}   

//Botão temorário para simular disparo. Pôr o código que está dentro, com as devidas alterações, no PistolWindow::processar(), quando eliminar-se o botão.
//Botão temorário para simular disparo.
void RifleWindow::on_ShootButton_clicked(){
    x=ui->doubleSpinBox->value();               //coordenadas x, substituir o que está depois do igual para as coordenadas obtidas pela camera.
    y=ui->doubleSpinBox_2->value();             //coordenadas y, substituir o que está depois do igual para as coordenadas obtidas pela camera.
    //intshot=                                  //pontuação sem casas decimais, descomentar e adicionar depois do igual a pontuação obtidas pela camera.
    //decshot=                                  //pontuação com casas decimais, descomentar e adicionar depois do igual a pontuação obtidas pela camera.
    QPixmap RedDot(":/resources/img/Red Dot.png");

    //if(procss==1){
        timedzoom.start(400); 

        //Inserir imagem
        item = new QGraphicsPixmapItem(RedDot);
        item->setScale(0.01);
        item->setPos(257.5+x,257.5+y);
        scene->addItem(item);


        //Código temporário para simular a pontuação. A ser eliminado quando já não for necessário.
        //10th circle
        if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=23*23){
            intshot=10;
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=23/10*23/10){
                decshot=10.9;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=2*23/10*2*23/10 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>23/10*23/10){
                decshot=10.8;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=3*23/10*3*23/10 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>2*23/10*2*23/10){
                decshot=10.7;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=4*23/10*4*23/10 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>3*23/10*3*23/10){
                decshot=10.6;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=5*23/10*5*23/10 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>4*23/10*4*23/10){
                decshot=10.5;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=6*23/10*6*23/10 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>5*23/10*5*23/10){
                decshot=10.4;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=7*23/10*7*23/10 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>6*23/10*6*23/10){
                decshot=10.3;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=8*23/10*8*23/10 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>7*23/10*7*23/10){
                decshot=10.2;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=9*23/10*9*23/10 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>8*23/10*8*23/10){
                decshot=10.1;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=23*23 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>9*23/10*9*23/10){
                decshot=10.0;
            }
        }

        //9th circle
        if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=48*48 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>23*23){
            intshot=9;
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(23+(48-23)/10)*(23+(48-23)/10)){
                decshot=9.9;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(23+2*(48-23)/10)*(23+2*(48-23)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(23+(48-23)/10)*(23+(48-23)/10)){
                decshot=9.8;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(23+3*(48-23)/10)*(23+3*(48-23)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(23+2*(48-23)/10)*(23+2*(48-23)/10)){
                decshot=9.7;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(23+4*(48-23)/10)*(23+4*(48-23)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(23+3*(48-23)/10)*(23+3*(48-23)/10)){
                decshot=9.6;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(23+5*(48-23)/10)*(23+5*(48-23)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(23+4*(48-23)/10)*(23+4*(48-23)/10)){
                decshot=9.5;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(23+6*(48-23)/10)*(23+6*(48-23)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(23+5*(48-23)/10)*(23+5*(48-23)/10)){
                decshot=9.4;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(23+7*(48-23)/10)*(23+7*(48-23)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(23+6*(48-23)/10)*(23+6*(48-23)/10)){
                decshot=9.3;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(23+8*(48-23)/10)*(23+8*(48-23)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(23+7*(48-23)/10)*(23+7*(48-23)/10)){
                decshot=9.2;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(23+9*(48-23)/10)*(23+9*(48-23)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(23+8*(48-23)/10)*(23+8*(48-23)/10)){
                decshot=9.1;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=48*48 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(23+9*(48-23)/10)*(23+9*(48-23)/10)){
                decshot=9.0;
            }
        }

        //8th circle
        if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=72*72 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>48*48){
            intshot=8;
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(48+(72-48)/10)*(48+(72-48)/10)){
                decshot=8.9;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(48+2*(72-48)/10)*(48+2*(72-48)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(48+(72-48)/10)*(48+(72-48)/10)){
                decshot=8.8;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(48+3*(72-48)/10)*(48+3*(72-48)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(48+2*(72-48)/10)*(48+2*(72-48)/10)){
                decshot=8.7;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(48+4*(72-48)/10)*(48+4*(72-48)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(48+3*(72-48)/10)*(48+3*(72-48)/10)){
                decshot=8.6;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(48+5*(72-48)/10)*(48+5*(72-48)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(48+4*(72-48)/10)*(48+4*(72-48)/10)){
                decshot=8.5;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(48+6*(72-48)/10)*(48+6*(72-48)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(48+5*(72-48)/10)*(48+5*(72-48)/10)){
                decshot=8.4;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(48+7*(72-48)/10)*(48+7*(72-48)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(48+6*(72-48)/10)*(48+6*(72-48)/10)){
                decshot=8.3;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(48+8*(72-48)/10)*(48+8*(72-48)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(48+7*(72-48)/10)*(48+7*(72-48)/10)){
                decshot=8.2;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(48+9*(72-48)/10)*(48+9*(72-48)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(48+8*(72-48)/10)*(48+8*(72-48)/10)){
                decshot=8.1;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=72*72 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(48+9*(72-48)/10)*(48+9*(72-48)/10)){
                decshot=8.0;
            }
        }

        //7th circle
        if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=96*96 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>72*72){
            intshot=7;
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(72+(96-72)/10)*(72+(96-72)/10)){
                decshot=7.9;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(72+2*(96-72)/10)*(72+2*(96-72)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(72+(96-72)/10)*(72+(96-72)/10)){
                decshot=7.8;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(72+3*(96-72)/10)*(72+3*(96-72)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(72+2*(96-72)/10)*(72+2*(96-72)/10)){
                decshot=7.7;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(72+4*(96-72)/10)*(72+4*(96-72)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(72+3*(96-72)/10)*(72+3*(96-72)/10)){
                decshot=7.6;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(72+5*(96-72)/10)*(72+5*(96-72)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(72+4*(96-72)/10)*(72+4*(96-72)/10)){
                decshot=7.5;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(72+6*(96-72)/10)*(72+6*(96-72)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(72+5*(96-72)/10)*(72+5*(96-72)/10)){
                decshot=7.4;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(72+7*(96-72)/10)*(72+7*(96-72)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(72+6*(96-72)/10)*(72+6*(96-72)/10)){
                decshot=7.3;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(72+8*(96-72)/10)*(72+8*(96-72)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(72+7*(96-72)/10)*(72+7*(96-72)/10)){
                decshot=7.2;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(72+9*(96-72)/10)*(72+9*(96-72)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(72+8*(96-72)/10)*(72+8*(96-72)/10)){
                decshot=7.1;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=96*96 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(72+9*(96-72)/10)*(72+9*(96-72)/10)){
                decshot=7.0;
            }
        }

        //6th circle
        if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=121*121 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>96*96){
            intshot=6;
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(96+(121-96)/10)*(96+(121-96)/10)){
                decshot=6.9;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(96+2*(121-96)/10)*(96+2*(121-96)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(96+(121-96)/10)*(96+(121-96)/10)){
                decshot=6.8;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(96+3*(121-96)/10)*(96+3*(121-96)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(96+2*(121-96)/10)*(96+2*(121-96)/10)){
                decshot=6.7;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(96+4*(121-96)/10)*(96+4*(121-96)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(96+3*(121-96)/10)*(96+3*(121-96)/10)){
                decshot=6.6;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(96+5*(121-96)/10)*(96+5*(121-96)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(96+4*(121-96)/10)*(96+4*(121-96)/10)){
                decshot=6.5;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(96+6*(121-96)/10)*(96+6*(121-96)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(96+5*(121-96)/10)*(96+5*(121-96)/10)){
                decshot=6.4;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(96+7*(121-96)/10)*(96+7*(121-96)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(96+6*(121-96)/10)*(96+6*(121-96)/10)){
                decshot=6.3;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(96+8*(121-96)/10)*(96+8*(121-96)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(96+7*(121-96)/10)*(96+7*(121-96)/10)){
                decshot=6.2;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(96+9*(121-96)/10)*(96+9*(121-96)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(96+8*(121-96)/10)*(96+8*(121-96)/10)){
                decshot=6.1;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=121*121 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(96+9*(121-96)/10)*(96+9*(121-96)/10)){
                decshot=6.0;
            }
        }

        //5th circle
        if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=145*145 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>121*121){
            intshot=5;
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(121+(145-121)/10)*(121+(145-121)/10)){
                decshot=5.9;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(121+2*(145-121)/10)*(121+2*(145-121)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(121+(145-121)/10)*(121+(145-121)/10)){
                decshot=5.8;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(121+3*(145-121)/10)*(121+3*(145-121)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(121+2*(145-121)/10)*(121+2*(145-121)/10)){
                decshot=5.7;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(121+4*(145-121)/10)*(121+4*(145-121)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(121+3*(145-121)/10)*(121+3*(145-121)/10)){
                decshot=5.6;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(121+5*(145-121)/10)*(121+5*(145-121)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(121+4*(145-121)/10)*(121+4*(145-121)/10)){
                decshot=5.5;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(121+6*(145-121)/10)*(121+6*(145-121)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(121+5*(145-121)/10)*(121+5*(145-121)/10)){
                decshot=5.4;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(121+7*(145-121)/10)*(121+7*(145-121)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(121+6*(145-121)/10)*(121+6*(145-121)/10)){
                decshot=5.3;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(121+8*(145-121)/10)*(121+8*(145-121)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(121+7*(145-121)/10)*(121+7*(145-121)/10)){
                decshot=5.2;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(121+9*(145-121)/10)*(121+9*(145-121)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(121+8*(145-121)/10)*(121+8*(145-121)/10)){
                decshot=5.1;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=145*145 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(121+9*(145-121)/10)*(121+9*(145-121)/10)){
                decshot=5.0;
            }
        }

        //4th circle
        if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=170*170 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>145*145){
            intshot=4;
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(145+(170-145)/10)*(145+(170-145)/10)){
                decshot=4.9;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(145+2*(170-145)/10)*(145+2*(170-145)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(145+(170-145)/10)*(145+(170-145)/10)){
                decshot=4.8;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(145+3*(170-145)/10)*(145+3*(170-145)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(145+2*(170-145)/10)*(145+2*(170-145)/10)){
                decshot=4.7;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(145+4*(170-145)/10)*(145+4*(170-145)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(145+3*(170-145)/10)*(145+3*(170-145)/10)){
                decshot=4.6;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(145+5*(170-145)/10)*(145+5*(170-145)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(145+4*(170-145)/10)*(145+4*(170-145)/10)){
                decshot=4.5;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(145+6*(170-145)/10)*(145+6*(170-145)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(145+5*(170-145)/10)*(145+5*(170-145)/10)){
                decshot=4.4;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(145+7*(170-145)/10)*(145+7*(170-145)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(145+6*(170-145)/10)*(145+6*(170-145)/10)){
                decshot=4.3;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(145+8*(170-145)/10)*(145+8*(170-145)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(145+7*(170-145)/10)*(145+7*(170-145)/10)){
                decshot=4.2;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(145+9*(170-145)/10)*(145+9*(170-145)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(145+8*(170-145)/10)*(145+8*(170-145)/10)){
                decshot=4.1;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=170*170 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(145+9*(170-145)/10)*(145+9*(170-145)/10)){
                decshot=4.0;
            }
        }

        //3rd circle
        if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=194*194 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>170*170){
            intshot=3;
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(170+(194-170)/10)*(170+(194-170)/10)){
                decshot=3.9;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(170+2*(194-170)/10)*(170+2*(194-170)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(170+(194-170)/10)*(170+(194-170)/10)){
                decshot=3.8;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(170+3*(194-170)/10)*(170+3*(194-170)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(170+2*(194-170)/10)*(170+2*(194-170)/10)){
                decshot=3.7;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(170+4*(194-170)/10)*(170+4*(194-170)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(170+3*(194-170)/10)*(170+3*(194-170)/10)){
                decshot=3.6;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(170+5*(194-170)/10)*(170+5*(194-170)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(170+4*(194-170)/10)*(170+4*(194-170)/10)){
                decshot=3.5;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(170+6*(194-170)/10)*(170+6*(194-170)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(170+5*(194-170)/10)*(170+5*(194-170)/10)){
                decshot=3.4;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(170+7*(194-170)/10)*(170+7*(194-170)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(170+6*(194-170)/10)*(170+6*(194-170)/10)){
                decshot=3.3;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(170+8*(194-170)/10)*(170+8*(194-170)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(170+7*(194-170)/10)*(170+7*(194-170)/10)){
                decshot=3.2;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(170+9*(194-170)/10)*(170+9*(194-170)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(170+8*(194-170)/10)*(170+8*(194-170)/10)){
                decshot=3.1;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=194*194 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(170+9*(194-170)/10)*(170+9*(194-170)/10)){
                decshot=3.0;
            }
        }

        //2nd circle
        if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=218*218  && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>194*194){
            intshot=2;
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(194+(218-194)/10)*(194+(218-194)/10)){
                decshot=2.9;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(194+2*(218-194)/10)*(194+2*(218-194)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(194+(218-194)/10)*(194+(218-194)/10)){
                decshot=2.8;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(194+3*(218-194)/10)*(194+3*(218-194)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(194+2*(218-194)/10)*(194+2*(218-194)/10)){
                decshot=2.7;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(194+4*(218-194)/10)*(194+4*(218-194)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(194+3*(218-194)/10)*(194+3*(218-194)/10)){
                decshot=2.6;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(194+5*(218-194)/10)*(194+5*(218-194)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(194+4*(218-194)/10)*(194+4*(218-194)/10)){
                decshot=2.5;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(194+6*(218-194)/10)*(194+6*(218-194)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(194+5*(218-194)/10)*(194+5*(218-194)/10)){
                decshot=2.4;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(194+7*(218-194)/10)*(194+7*(218-194)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(194+6*(218-194)/10)*(194+6*(218-194)/10)){
                decshot=2.3;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(194+8*(218-194)/10)*(194+8*(218-194)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(194+7*(218-194)/10)*(194+7*(218-194)/10)){
                decshot=2.2;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(194+9*(218-194)/10)*(194+9*(218-194)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(194+8*(218-194)/10)*(194+8*(218-194)/10)){
                decshot=2.1;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=218*218 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(194+9*(218-194)/10)*(194+9*(218-194)/10)){
                decshot=2.0;
            }
        }

        //1st circle
        if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=243*243 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>218*218){
            intshot=1;
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(218+(243-218)/10)*(218+(243-218)/10)){
                decshot=1.9;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(218+2*(243-218)/10)*(218+2*(243-218)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(218+(243-218)/10)*(218+(243-218)/10)){
                decshot=1.8;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(218+3*(243-218)/10)*(218+3*(243-218)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(218+2*(243-218)/10)*(218+2*(243-218)/10)){
                decshot=1.7;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(218+4*(243-218)/10)*(218+4*(243-218)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(218+3*(243-218)/10)*(218+3*(243-218)/10)){
                decshot=1.6;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(218+5*(243-218)/10)*(218+5*(243-218)/10)&& ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(218+4*(243-218)/10)*(218+4*(243-218)/10)){
                decshot=1.5;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(218+6*(243-218)/10)*(218+6*(243-218)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(218+5*(243-218)/10)*(218+5*(243-218)/10)){
                decshot=1.4;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(218+7*(243-218)/10)*(218+7*(243-218)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(218+6*(243-218)/10)*(218+6*(243-218)/10)){
                decshot=1.3;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(218+8*(243-218)/10)*(218+8*(243-218)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(218+7*(243-218)/10)*(218+7*(243-218)/10)){
                decshot=1.2;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=(218+9*(243-218)/10)*(218+9*(243-218)/10) && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(218+8*(243-218)/10)*(218+8*(243-218)/10)){
                decshot=1.1;
            }
            if(((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)<=243*243 && ((x+257.5)-257.5)*((x+257.5)-257.5)+((y+257.5)-257.5)*((y+257.5)-257.5)>(218+9*(243-218)/10)*(218+9*(243-218)/10)){
                decshot=1.0;
            }
        }

        //Código para preencher a tabela e os labels abaixo da tabela.
        totalintshot=totalintshot+intshot;
        totaldecshot=totaldecshot+decshot;
        ui->lastshot->setText(QString::number(decshot));
        ui->total_int->setText(QString::number(nim));
        ui->total_dec->setText(QString::number(totalintshot));
        ui->total_dec_2->setText(QString::number(totaldecshot));
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,Tiro,new QTableWidgetItem(QString::number(nim)));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,Inte,new QTableWidgetItem(QString::number(intshot)));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,Dec,new QTableWidgetItem(QString::number(decshot)));


        //Código para fazer zoom onde houve o disparo.
        if(timezoom==0){
            ui->Target->setTransform(QTransform::fromScale(5,5));
            ui->Target->centerOn(QPointF(259.0+x,259.0+y));
        }
        
        nim=nim+1;
    //}
}

//Código para alternar o ponto entre vermelho e azul enquanto está a fazer zoom onde houve o disparo. Também termina o zoom após algum tempo.
void RifleWindow::shootzoom()
{   
    QPixmap RedDot(":/resources/img/Red Dot.png");
    QPixmap BlueDot(":/resources/img/Blue Dot.png");
    timezoom++;
    if(timezoom%2==0 && timezoom<=4){
        item=new QGraphicsPixmapItem(RedDot);
        item->setScale(0.01);
        item->setPos(257.5+x,257.5+y);
        scene->addItem(item);
    }
    if(timezoom%2==1 && timezoom<=4){
        item=new QGraphicsPixmapItem(BlueDot);
        item->setScale(0.01);
        item->setPos(257.5+x,257.5+y);
        scene->addItem(item);
    }
    if(timezoom==4){
        timedzoom.stop();
        ui->Target->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
        timezoom=0;
    }
}


// Código para limpar o Target
//    qDeleteAll(scene->items());
//    QPixmap PistolTarget(":/resources/img/RifleTarget.png");
//    scene->addPixmap(PistolTarget.scaled(w,h));
//    ui->Target->setScene(scene);