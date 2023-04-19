#include "pistolwindow.h"
#include "ui_pistolwindow.h"
#include <QGraphicsPixmapItem>
#include <QResizeEvent>

PistolWindow::PistolWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PistolWindow)
{
    ui->setupUi(this);

    int w = ui->Target->width();
    int h = ui->Target->height();
    QPixmap PistolTarget(":/resources/img/PistolTarget.png");
    scene = new QGraphicsScene(this);
    scene->addPixmap(PistolTarget.scaled(w-2,h-2));
    ui->Target->setScene(scene);

    ui->ExitButton->setIcon(QIcon(":/resources/img/exit.png"));

    QStringList titulos; 
    ui->tableWidget->setColumnCount(2);
    titulos << "Int" << "Dec";
    ui->tableWidget->setHorizontalHeaderLabels(titulos);

    segundos=0;
    minutos=0;
    horas=0;
    procss=0;
    al=0;
    nim=1;
    zoom=0;
    intshot=0;
    totalintshot=0;
    decshot=0;
    totaldecshot=0;
    connect(&reloj,SIGNAL(timeout()),this,SLOT(processar()));
    connect(&alert,SIGNAL(timeout()),this,SLOT(alerta()));

    QObject::connect(this, &PistolWindow::finalButtonClickedSignal, this, &PistolWindow::finalButtonClicked);
    QObject::connect(this, &PistolWindow::practiceButtonClickedSignal, this, &PistolWindow::practiceButtonClicked);
    QObject::connect(this, &PistolWindow::matchButtonClickedSignal, this, &PistolWindow::matchButtonClicked);
    QObject::connect(this, &PistolWindow::startButtonClickedSignal, this, &PistolWindow::startButtonClicked);
}

PistolWindow::~PistolWindow()
{
    delete ui;
}

void PistolWindow::startButtonClicked()
{  
   reloj.start(1000);
   alert.start(250);
   procss=1;
}

//Alterar os botões pelos que vão ser usados no pc _central, adicionar um include ao .h do ui onde os botões se encontram.
void PistolWindow::practiceButtonClicked()
{  
   if(procss==0){
       segundos=2;
       minutos=1;
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

void PistolWindow::matchButtonClicked()
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

void PistolWindow::finalButtonClicked()
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

void PistolWindow::on_MainButton_clicked(){
    this->hide();
    this->parentWidget()->show();
}

void PistolWindow::on_ExitButton_clicked()
{
    qApp->quit();
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

void PistolWindow::showEvent(QShowEvent *revent){
    ui->Target->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}

void PistolWindow::resizeEvent(QResizeEvent *event){
    if(zoom==0){
        ui->Target->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    }
    else{
        ui->Target->centerOn(QPointF(253.0,253.0));
    }
    QMainWindow::resizeEvent(event);
}

void PistolWindow::on_horizontalSlider_valueChanged(int value){
    ui->Target->setTransform(QTransform::fromScale(value/100,value/100));
    if(value/100==1){
        ui->Target->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
        zoom=0;
    }
    else{
        zoom=1;
    }
}   


//Botão temorário para simular disparo, ainda não tem pontuações.
void PistolWindow::on_ShootButton_clicked(){
    x=ui->doubleSpinBox->value();
    y=ui->doubleSpinBox_2->value();
    if (nim==1){
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item->setScale(0.01);
        item->setPos(253+x,253+y);
        scene->addItem(item);
    }
    if (nim==2){
        QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item2->setScale(0.01);
        item2->setPos(253+x,253+y);
        scene->addItem(item2);
    }
    if (nim==3){
        QGraphicsPixmapItem* item3 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item3->setScale(0.01);
        item3->setPos(253+x,253+y);
        scene->addItem(item3);
    }
    if (nim==4){
        QGraphicsPixmapItem* item4 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item4->setScale(0.01);
        item4->setPos(253+x,253+y);
        scene->addItem(item4);
    }
    if (nim==5){
        QGraphicsPixmapItem* item5 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item5->setScale(0.01);
        item5->setPos(253+x,253+y);
        scene->addItem(item5);
    }
    if (nim==6){
        QGraphicsPixmapItem* item6 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item6->setScale(0.01);
        item6->setPos(253+x,253+y);
        scene->addItem(item6);
    }
    if (nim==7){
        QGraphicsPixmapItem* item7 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item7->setScale(0.01);
        item7->setPos(253+x,253+y);
        scene->addItem(item7);
    }
    if (nim==8){
        QGraphicsPixmapItem* item8 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item8->setScale(0.01);
        item8->setPos(253+x,253+y);
        scene->addItem(item8);
    }
    if (nim==9){
        QGraphicsPixmapItem* item9 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item9->setScale(0.01);
        item9->setPos(253+x,253+y);
        scene->addItem(item9);
    }
    if (nim==10){
        QGraphicsPixmapItem* item10 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item10->setScale(0.01);
        item10->setPos(253+x,253+y);
        scene->addItem(item10);
    }
    if (nim==11){
        QGraphicsPixmapItem* item11 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item11->setScale(0.01);
        item11->setPos(253+x,253+y);
        scene->addItem(item11);
    }
    if (nim==12){
        QGraphicsPixmapItem* item12 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item12->setScale(0.01);
        item12->setPos(253+x,253+y);
        scene->addItem(item12);
    }
    if (nim==13){
        QGraphicsPixmapItem* item13 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item13->setScale(0.01);
        item13->setPos(253+x,253+y);
        scene->addItem(item13);
    }
    if (nim==14){
        QGraphicsPixmapItem* item14 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item14->setScale(0.01);
        item14->setPos(253+x,253+y);
        scene->addItem(item14);
    }
    if (nim==15){
        QGraphicsPixmapItem* item15 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item15->setScale(0.01);
        item15->setPos(253+x,253+y);
        scene->addItem(item15);
    }
    if (nim==16){
        QGraphicsPixmapItem* item16 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item16->setScale(0.01);
        item16->setPos(253+x,253+y);
        scene->addItem(item16);
    }
    if (nim==17){
        QGraphicsPixmapItem* item17 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item17->setScale(0.01);
        item17->setPos(253+x,253+y);
        scene->addItem(item17);
    }
    if (nim==18){
        QGraphicsPixmapItem* item18 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item18->setScale(0.01);
        item18->setPos(253+x,253+y);
        scene->addItem(item18);
    }
    if (nim==19){
        QGraphicsPixmapItem* item19 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item19->setScale(0.01);
        item19->setPos(253+x,253+y);
        scene->addItem(item19);
    }
    if (nim==20){
        QGraphicsPixmapItem* item20 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item20->setScale(0.01);
        item20->setPos(253+x,253+y);
        scene->addItem(item20);
    }
    if (nim==21){
        QGraphicsPixmapItem* item21 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item21->setScale(0.01);
        item21->setPos(253+x,253+y);
        scene->addItem(item21);
    }
    if (nim==22){
        QGraphicsPixmapItem* item22 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item22->setScale(0.01);
        item22->setPos(253+x,253+y);
        scene->addItem(item22);
    }
    if (nim==23){
        QGraphicsPixmapItem* item23 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item23->setScale(0.01);
        item23->setPos(253+x,253+y);
        scene->addItem(item23);
    }
    if (nim==24){
        QGraphicsPixmapItem* item24 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item24->setScale(0.01);
        item24->setPos(253+x,253+y);
        scene->addItem(item24);
    }
    if (nim==25){
        QGraphicsPixmapItem* item25 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item25->setScale(0.01);
        item25->setPos(253+x,253+y);
        scene->addItem(item25);
    }
    if (nim==26){
        QGraphicsPixmapItem* item26 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item26->setScale(0.01);
        item26->setPos(253+x,253+y);
        scene->addItem(item26);
    }
    if (nim==27){
        QGraphicsPixmapItem* item27 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item27->setScale(0.01);
        item27->setPos(253+x,253+y);
        scene->addItem(item27);
    }
    if (nim==28){
        QGraphicsPixmapItem* item28 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item28->setScale(0.01);
        item28->setPos(253+x,253+y);
        scene->addItem(item28);
    }
    if (nim==29){
        QGraphicsPixmapItem* item29 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item29->setScale(0.01);
        item29->setPos(253+x,253+y);
        scene->addItem(item29);
    }
    if (nim==30){
        QGraphicsPixmapItem* item30 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item30->setScale(0.01);
        item30->setPos(253+x,253+y);
        scene->addItem(item30);
    }
    if (nim==31){
        QGraphicsPixmapItem* item31 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item31->setScale(0.01);
        item31->setPos(253+x,253+y);
        scene->addItem(item31);
    }
    if (nim==32){
        QGraphicsPixmapItem* item32 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item32->setScale(0.01);
        item32->setPos(253+x,253+y);
        scene->addItem(item32);
    }
    if (nim==33){
        QGraphicsPixmapItem* item33 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item33->setScale(0.01);
        item33->setPos(253+x,253+y);
        scene->addItem(item33);
    }
    if (nim==34){
        QGraphicsPixmapItem* item34 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item34->setScale(0.01);
        item34->setPos(253+x,253+y);
        scene->addItem(item34);
    }
    if (nim==35){
        QGraphicsPixmapItem* item35 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item35->setScale(0.01);
        item35->setPos(253+x,253+y);
        scene->addItem(item35);
    }
    if (nim==36){
        QGraphicsPixmapItem* item36 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item36->setScale(0.01);
        item36->setPos(253+x,253+y);
        scene->addItem(item36);
    }
    if (nim==37){
        QGraphicsPixmapItem* item37 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item37->setScale(0.01);
        item37->setPos(253+x,253+y);
        scene->addItem(item37);
    }
    if (nim==38){
        QGraphicsPixmapItem* item38 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item38->setScale(0.01);
        item38->setPos(253+x,253+y);
        scene->addItem(item38);
    }
    if (nim==39){
        QGraphicsPixmapItem* item39 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item39->setScale(0.01);
        item39->setPos(253+x,253+y);
        scene->addItem(item39);
    }
    if (nim==40){
        QGraphicsPixmapItem* item40 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item40->setScale(0.01);
        item40->setPos(253+x,253+y);
        scene->addItem(item40);
    }
    if (nim==41){
        QGraphicsPixmapItem* item41 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item41->setScale(0.01);
        item41->setPos(253+x,253+y);
        scene->addItem(item41);
    }
    if (nim==42){
        QGraphicsPixmapItem* item42 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item42->setScale(0.01);
        item42->setPos(253+x,253+y);
        scene->addItem(item42);
    }
    if (nim==43){
        QGraphicsPixmapItem* item43 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item43->setScale(0.01);
        item43->setPos(253+x,253+y);
        scene->addItem(item43);
    }
    if (nim==44){
        QGraphicsPixmapItem* item44 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item44->setScale(0.01);
        item44->setPos(253+x,253+y);
        scene->addItem(item44);
    }
    if (nim==45){
        QGraphicsPixmapItem* item45 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item45->setScale(0.01);
        item45->setPos(253+x,253+y);
        scene->addItem(item45);
    }
    if (nim==46){
        QGraphicsPixmapItem* item46 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item46->setScale(0.01);
        item46->setPos(253+x,253+y);
        scene->addItem(item46);
    }
    if (nim==47){
        QGraphicsPixmapItem* item47 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item47->setScale(0.01);
        item47->setPos(253+x,253+y);
        scene->addItem(item47);
    }
    if (nim==48){
        QGraphicsPixmapItem* item48 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item48->setScale(0.01);
        item48->setPos(253+x,253+y);
        scene->addItem(item48);
    }
    if (nim==49){
        QGraphicsPixmapItem* item49 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item49->setScale(0.01);
        item49->setPos(253+x,253+y);
        scene->addItem(item49);
    }
    if (nim==50){
        QGraphicsPixmapItem* item50 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item50->setScale(0.01);
        item50->setPos(253+x,253+y);
        scene->addItem(item50);
    }
    if (nim==51){
        QGraphicsPixmapItem* item51 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item51->setScale(0.01);
        item51->setPos(253+x,253+y);
        scene->addItem(item51);
    }
    if (nim==52){
        QGraphicsPixmapItem* item52 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item52->setScale(0.01);
        item52->setPos(253+x,253+y);
        scene->addItem(item52);
    }
    if (nim==53){
        QGraphicsPixmapItem* item53 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item53->setScale(0.01);
        item53->setPos(253+x,253+y);
        scene->addItem(item53);
    }
    if (nim==54){
        QGraphicsPixmapItem* item54 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item54->setScale(0.01);
        item54->setPos(253+x,253+y);
        scene->addItem(item54);
    }
    if (nim==55){
        QGraphicsPixmapItem* item55 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item55->setScale(0.01);
        item55->setPos(253+x,253+y);
        scene->addItem(item55);
    }
    if (nim==56){
        QGraphicsPixmapItem* item56 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item56->setScale(0.01);
        item56->setPos(253+x,253+y);
        scene->addItem(item56);
    }
    if (nim==57){
        QGraphicsPixmapItem* item57 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item57->setScale(0.01);
        item57->setPos(253+x,253+y);
        scene->addItem(item57);
    }
    if (nim==58){
        QGraphicsPixmapItem* item58 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item58->setScale(0.01);
        item58->setPos(253+x,253+y);
        scene->addItem(item58);
    }
    if (nim==59){
        QGraphicsPixmapItem* item59 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item59->setScale(0.01);
        item59->setPos(253+x,253+y);
        scene->addItem(item59);
    }
    if (nim==60){
        QGraphicsPixmapItem* item60 = new QGraphicsPixmapItem(QPixmap(":/resources/img/exit.png"));
        item60->setScale(0.01);
        item60->setPos(253+x,253+y);
        scene->addItem(item60);
    }

    //10th circle
    if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=23*23){
        intshot=10;
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=23/10*23/10){
            decshot=10.9;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=2*23/10*2*23/10 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>23/10*23/10){
            decshot=10.8;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=3*23/10*3*23/10 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>2*23/10*2*23/10){
            decshot=10.7;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=4*23/10*4*23/10 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>3*23/10*3*23/10){
            decshot=10.6;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=5*23/10*5*23/10 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>4*23/10*4*23/10){
            decshot=10.5;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=6*23/10*6*23/10 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>5*23/10*5*23/10){
            decshot=10.4;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=7*23/10*7*23/10 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>6*23/10*6*23/10){
            decshot=10.3;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=8*23/10*8*23/10 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>7*23/10*7*23/10){
            decshot=10.2;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=9*23/10*9*23/10 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>8*23/10*8*23/10){
            decshot=10.1;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=23*23 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>9*23/10*9*23/10){
            decshot=10.0;
        }
    }

    //9th circle
    if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=48*48 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>23*23){
        intshot=9;
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(23+(48-23)/10)*(23+(48-23)/10)){
            decshot=9.9;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(23+2*(48-23)/10)*(23+2*(48-23)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(23+(48-23)/10)*(23+(48-23)/10)){
            decshot=9.8;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(23+3*(48-23)/10)*(23+3*(48-23)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(23+2*(48-23)/10)*(23+2*(48-23)/10)){
            decshot=9.7;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(23+4*(48-23)/10)*(23+4*(48-23)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(23+3*(48-23)/10)*(23+3*(48-23)/10)){
            decshot=9.6;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(23+5*(48-23)/10)*(23+5*(48-23)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(23+4*(48-23)/10)*(23+4*(48-23)/10)){
            decshot=9.5;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(23+6*(48-23)/10)*(23+6*(48-23)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(23+5*(48-23)/10)*(23+5*(48-23)/10)){
            decshot=9.4;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(23+7*(48-23)/10)*(23+7*(48-23)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(23+6*(48-23)/10)*(23+6*(48-23)/10)){
            decshot=9.3;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(23+8*(48-23)/10)*(23+8*(48-23)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(23+7*(48-23)/10)*(23+7*(48-23)/10)){
            decshot=9.2;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(23+9*(48-23)/10)*(23+9*(48-23)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(23+8*(48-23)/10)*(23+8*(48-23)/10)){
            decshot=9.1;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=48*48 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(23+9*(48-23)/10)*(23+9*(48-23)/10)){
            decshot=9.0;
        }
    }

    //8th circle
    if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=72*72 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>48*48){
        intshot=8;
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(48+(72-48)/10)*(48+(72-48)/10)){
            decshot=8.9;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(48+2*(72-48)/10)*(48+2*(72-48)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(48+(72-48)/10)*(48+(72-48)/10)){
            decshot=8.8;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(48+3*(72-48)/10)*(48+3*(72-48)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(48+2*(72-48)/10)*(48+2*(72-48)/10)){
            decshot=8.7;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(48+4*(72-48)/10)*(48+4*(72-48)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(48+3*(72-48)/10)*(48+3*(72-48)/10)){
            decshot=8.6;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(48+5*(72-48)/10)*(48+5*(72-48)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(48+4*(72-48)/10)*(48+4*(72-48)/10)){
            decshot=8.5;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(48+6*(72-48)/10)*(48+6*(72-48)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(48+5*(72-48)/10)*(48+5*(72-48)/10)){
            decshot=8.4;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(48+7*(72-48)/10)*(48+7*(72-48)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(48+6*(72-48)/10)*(48+6*(72-48)/10)){
            decshot=8.3;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(48+8*(72-48)/10)*(48+8*(72-48)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(48+7*(72-48)/10)*(48+7*(72-48)/10)){
            decshot=8.2;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(48+9*(72-48)/10)*(48+9*(72-48)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(48+8*(72-48)/10)*(48+8*(72-48)/10)){
            decshot=8.1;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=72*72 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(48+9*(72-48)/10)*(48+9*(72-48)/10)){
            decshot=8.0;
        }
    }

    //7th circle
    if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=96*96 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>72*72){
        intshot=7;
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(72+(96-72)/10)*(72+(96-72)/10)){
            decshot=7.9;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(72+2*(96-72)/10)*(72+2*(96-72)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(72+(96-72)/10)*(72+(96-72)/10)){
            decshot=7.8;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(72+3*(96-72)/10)*(72+3*(96-72)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(72+2*(96-72)/10)*(72+2*(96-72)/10)){
            decshot=7.7;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(72+4*(96-72)/10)*(72+4*(96-72)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(72+3*(96-72)/10)*(72+3*(96-72)/10)){
            decshot=7.6;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(72+5*(96-72)/10)*(72+5*(96-72)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(72+4*(96-72)/10)*(72+4*(96-72)/10)){
            decshot=7.5;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(72+6*(96-72)/10)*(72+6*(96-72)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(72+5*(96-72)/10)*(72+5*(96-72)/10)){
            decshot=7.4;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(72+7*(96-72)/10)*(72+7*(96-72)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(72+6*(96-72)/10)*(72+6*(96-72)/10)){
            decshot=7.3;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(72+8*(96-72)/10)*(72+8*(96-72)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(72+7*(96-72)/10)*(72+7*(96-72)/10)){
            decshot=7.2;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(72+9*(96-72)/10)*(72+9*(96-72)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(72+8*(96-72)/10)*(72+8*(96-72)/10)){
            decshot=7.1;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=96*96 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(72+9*(96-72)/10)*(72+9*(96-72)/10)){
            decshot=7.0;
        }
    }

    //6th circle
    if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=121*121 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>96*96){
        intshot=6;
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(96+(121-96)/10)*(96+(121-96)/10)){
            decshot=6.9;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(96+2*(121-96)/10)*(96+2*(121-96)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(96+(121-96)/10)*(96+(121-96)/10)){
            decshot=6.8;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(96+3*(121-96)/10)*(96+3*(121-96)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(96+2*(121-96)/10)*(96+2*(121-96)/10)){
            decshot=6.7;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(96+4*(121-96)/10)*(96+4*(121-96)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(96+3*(121-96)/10)*(96+3*(121-96)/10)){
            decshot=6.6;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(96+5*(121-96)/10)*(96+5*(121-96)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(96+4*(121-96)/10)*(96+4*(121-96)/10)){
            decshot=6.5;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(96+6*(121-96)/10)*(96+6*(121-96)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(96+5*(121-96)/10)*(96+5*(121-96)/10)){
            decshot=6.4;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(96+7*(121-96)/10)*(96+7*(121-96)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(96+6*(121-96)/10)*(96+6*(121-96)/10)){
            decshot=6.3;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(96+8*(121-96)/10)*(96+8*(121-96)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(96+7*(121-96)/10)*(96+7*(121-96)/10)){
            decshot=6.2;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(96+9*(121-96)/10)*(96+9*(121-96)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(96+8*(121-96)/10)*(96+8*(121-96)/10)){
            decshot=6.1;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=121*121 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(96+9*(121-96)/10)*(96+9*(121-96)/10)){
            decshot=6.0;
        }
    }

    //5th circle
    if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=145*145 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>121*121){
        intshot=5;
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(121+(145-121)/10)*(121+(145-121)/10)){
            decshot=5.9;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(121+2*(145-121)/10)*(121+2*(145-121)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(121+(145-121)/10)*(121+(145-121)/10)){
            decshot=5.8;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(121+3*(145-121)/10)*(121+3*(145-121)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(121+2*(145-121)/10)*(121+2*(145-121)/10)){
            decshot=5.7;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(121+4*(145-121)/10)*(121+4*(145-121)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(121+3*(145-121)/10)*(121+3*(145-121)/10)){
            decshot=5.6;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(121+5*(145-121)/10)*(121+5*(145-121)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(121+4*(145-121)/10)*(121+4*(145-121)/10)){
            decshot=5.5;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(121+6*(145-121)/10)*(121+6*(145-121)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(121+5*(145-121)/10)*(121+5*(145-121)/10)){
            decshot=5.4;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(121+7*(145-121)/10)*(121+7*(145-121)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(121+6*(145-121)/10)*(121+6*(145-121)/10)){
            decshot=5.3;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(121+8*(145-121)/10)*(121+8*(145-121)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(121+7*(145-121)/10)*(121+7*(145-121)/10)){
            decshot=5.2;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(121+9*(145-121)/10)*(121+9*(145-121)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(121+8*(145-121)/10)*(121+8*(145-121)/10)){
            decshot=5.1;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=145*145 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(121+9*(145-121)/10)*(121+9*(145-121)/10)){
            decshot=5.0;
        }
    }

    //4th circle
    if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=170*170 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>145*145){
        intshot=4;
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(145+(170-145)/10)*(145+(170-145)/10)){
            decshot=4.9;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(145+2*(170-145)/10)*(145+2*(170-145)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(145+(170-145)/10)*(145+(170-145)/10)){
            decshot=4.8;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(145+3*(170-145)/10)*(145+3*(170-145)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(145+2*(170-145)/10)*(145+2*(170-145)/10)){
            decshot=4.7;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(145+4*(170-145)/10)*(145+4*(170-145)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(145+3*(170-145)/10)*(145+3*(170-145)/10)){
            decshot=4.6;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(145+5*(170-145)/10)*(145+5*(170-145)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(145+4*(170-145)/10)*(145+4*(170-145)/10)){
            decshot=4.5;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(145+6*(170-145)/10)*(145+6*(170-145)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(145+5*(170-145)/10)*(145+5*(170-145)/10)){
            decshot=4.4;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(145+7*(170-145)/10)*(145+7*(170-145)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(145+6*(170-145)/10)*(145+6*(170-145)/10)){
            decshot=4.3;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(145+8*(170-145)/10)*(145+8*(170-145)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(145+7*(170-145)/10)*(145+7*(170-145)/10)){
            decshot=4.2;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(145+9*(170-145)/10)*(145+9*(170-145)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(145+8*(170-145)/10)*(145+8*(170-145)/10)){
            decshot=4.1;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=170*170 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(145+9*(170-145)/10)*(145+9*(170-145)/10)){
            decshot=4.0;
        }
    }

    //3rd circle
    if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=194*194 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>170*170){
        intshot=3;
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(170+(194-170)/10)*(170+(194-170)/10)){
            decshot=3.9;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(170+2*(194-170)/10)*(170+2*(194-170)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(170+(194-170)/10)*(170+(194-170)/10)){
            decshot=3.8;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(170+3*(194-170)/10)*(170+3*(194-170)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(170+2*(194-170)/10)*(170+2*(194-170)/10)){
            decshot=3.7;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(170+4*(194-170)/10)*(170+4*(194-170)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(170+3*(194-170)/10)*(170+3*(194-170)/10)){
            decshot=3.6;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(170+5*(194-170)/10)*(170+5*(194-170)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(170+4*(194-170)/10)*(170+4*(194-170)/10)){
            decshot=3.5;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(170+6*(194-170)/10)*(170+6*(194-170)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(170+5*(194-170)/10)*(170+5*(194-170)/10)){
            decshot=3.4;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(170+7*(194-170)/10)*(170+7*(194-170)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(170+6*(194-170)/10)*(170+6*(194-170)/10)){
            decshot=3.3;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(170+8*(194-170)/10)*(170+8*(194-170)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(170+7*(194-170)/10)*(170+7*(194-170)/10)){
            decshot=3.2;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(170+9*(194-170)/10)*(170+9*(194-170)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(170+8*(194-170)/10)*(170+8*(194-170)/10)){
            decshot=3.1;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=194*194 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(170+9*(194-170)/10)*(170+9*(194-170)/10)){
            decshot=3.0;
        }
    }

    //2nd circle
    if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=218*218  && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>194*194){
        intshot=2;
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(194+(218-194)/10)*(194+(218-194)/10)){
            decshot=2.9;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(194+2*(218-194)/10)*(194+2*(218-194)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(194+(218-194)/10)*(194+(218-194)/10)){
            decshot=2.8;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(194+3*(218-194)/10)*(194+3*(218-194)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(194+2*(218-194)/10)*(194+2*(218-194)/10)){
            decshot=2.7;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(194+4*(218-194)/10)*(194+4*(218-194)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(194+3*(218-194)/10)*(194+3*(218-194)/10)){
            decshot=2.6;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(194+5*(218-194)/10)*(194+5*(218-194)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(194+4*(218-194)/10)*(194+4*(218-194)/10)){
            decshot=2.5;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(194+6*(218-194)/10)*(194+6*(218-194)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(194+5*(218-194)/10)*(194+5*(218-194)/10)){
            decshot=2.4;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(194+7*(218-194)/10)*(194+7*(218-194)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(194+6*(218-194)/10)*(194+6*(218-194)/10)){
            decshot=2.3;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(194+8*(218-194)/10)*(194+8*(218-194)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(194+7*(218-194)/10)*(194+7*(218-194)/10)){
            decshot=2.2;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(194+9*(218-194)/10)*(194+9*(218-194)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(194+8*(218-194)/10)*(194+8*(218-194)/10)){
            decshot=2.1;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=218*218 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(194+9*(218-194)/10)*(194+9*(218-194)/10)){
            decshot=2.0;
        }
    }

    //1st circle
    if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=243*243 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>218*218){
        intshot=1;
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(218+(243-218)/10)*(218+(243-218)/10)){
            decshot=1.9;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(218+2*(243-218)/10)*(218+2*(243-218)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(218+(243-218)/10)*(218+(243-218)/10)){
            decshot=1.8;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(218+3*(243-218)/10)*(218+3*(243-218)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(218+2*(243-218)/10)*(218+2*(243-218)/10)){
            decshot=1.7;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(218+4*(243-218)/10)*(218+4*(243-218)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(218+3*(243-218)/10)*(218+3*(243-218)/10)){
            decshot=1.6;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(218+5*(243-218)/10)*(218+5*(243-218)/10)&& ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(218+4*(243-218)/10)*(218+4*(243-218)/10)){
            decshot=1.5;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(218+6*(243-218)/10)*(218+6*(243-218)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(218+5*(243-218)/10)*(218+5*(243-218)/10)){
            decshot=1.4;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(218+7*(243-218)/10)*(218+7*(243-218)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(218+6*(243-218)/10)*(218+6*(243-218)/10)){
            decshot=1.3;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(218+8*(243-218)/10)*(218+8*(243-218)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(218+7*(243-218)/10)*(218+7*(243-218)/10)){
            decshot=1.2;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=(218+9*(243-218)/10)*(218+9*(243-218)/10) && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(218+8*(243-218)/10)*(218+8*(243-218)/10)){
            decshot=1.1;
        }
        if(((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)<=243*243 && ((x+253)-253)*((x+253)-253)+((y+253)-253)*((y+253)-253)>(218+9*(243-218)/10)*(218+9*(243-218)/10)){
            decshot=1.0;
        }
    }

    totalintshot=totalintshot+intshot;
    totaldecshot=totaldecshot+decshot;
    ui->lastshot->setText(QString::number(intshot));
    ui->total_int->setText(QString::number(nim));
    ui->total_dec->setText(QString::number(totalintshot));
    ui->total_dec_2->setText(QString::number(totaldecshot));
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,Int,new QTableWidgetItem(QString::number(intshot)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,Dec,new QTableWidgetItem(QString::number(decshot)));

    nim=nim+1;
}