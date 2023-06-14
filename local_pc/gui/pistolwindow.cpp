#include "pistolwindow.h"
#include "ui_pistolwindow.h"
#include <QGraphicsPixmapItem>

PistolWindow::PistolWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PistolWindow)
{
    ui->setupUi(this);

    float w = ui->Target->width();
    float h = ui->Target->height();
    QImage PistolTarget(":/resources/img/PistolTarget.png");
    scene = new QGraphicsScene(this);
    scene->addPixmap(QPixmap::fromImage(PistolTarget));
    ui->Target->setScene(scene);

    ui->ExitButton->setIcon(QIcon(":/resources/img/exit.png"));

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

    QObject::connect(this, &PistolWindow::finalButtonClickedSignal, this, &PistolWindow::finalButtonClicked);
    QObject::connect(this, &PistolWindow::practiceButtonClickedSignal, this, &PistolWindow::practiceButtonClicked);
    QObject::connect(this, &PistolWindow::matchButtonClickedSignal, this, &PistolWindow::matchButtonClicked);
    QObject::connect(this, &PistolWindow::startButtonClickedSignal, this, &PistolWindow::startButtonClicked);
    QObject::connect(this, &PistolWindow::backToDecideModeSignal, this, &PistolWindow::backToDecideMode);
    QObject::connect(this, &PistolWindow::backToDecideTypeSignal, this, &PistolWindow::backToDecideType);
    QObject::connect(this, &PistolWindow::new_score, this, &PistolWindow::Disparo);
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

void PistolWindow::practiceButtonClicked()
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
        ui->Target->centerOn(QPointF(465.0,465.0));
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

void PistolWindow::resetTimer()
{  
    reloj.stop();
    procss=0;
    segundos=0;
    minutos=0;
    horas=0;
    ui->seconds->display(segundos);
    ui->minutes->display(minutos);
    ui->hours->display(horas);
    ui->Start->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
    ui->Practice->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
    ui->Match->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
    ui->Final->setStyleSheet("QLabel{background-color: rgb(100, 100, 100)}");
}

void PistolWindow::backToDecideMode(){
    resetTimer();
    deleteShots();
    limparTabela();
}

void PistolWindow::backToDecideType(){
    emit backButtonClickedSignal();
    resetTimer();
    this->close();
}


void PistolWindow::Disparo(int coordenada_x, int coordenada_y, double pontuação){
    deleteShots();

    x=coordenada_x;                                         //coordenadas x, substituir o que está depois do igual para as coordenadas obtidas pela camera.
    y=coordenada_y;                                         //coordenadas y, substituir o que está depois do igual para as coordenadas obtidas pela camera.
    intshot= static_cast<int>(pontuação);                   //pontuação sem casas decimais, descomentar e adicionar depois do igual a pontuação obtidas pela camera.
    decshot=pontuação;                                      //pontuação com casas decimais, descomentar e adicionar depois do igual a pontuação obtidas pela camera.
    QPixmap RedDot(":/resources/img/Red Dot.png");
    
    if(procss==1){
        timedzoom.start(400); 
        
        //Inserir imagem
        item = new QGraphicsPixmapItem(RedDot);
        item->setScale(0.04);
        item->setPos(x,y);          //Pode ser necessário fazer ajustes por não estarem na mesma escala.
        scene->addItem(item);


        //Código para preencher a tabela e os labels abaixo da tabela.
        totalintshot=totalintshot+intshot;
        totaldecshot=totaldecshot+decshot;
        ui->lastshot->setText(QString::number(intshot));
        ui->total_int->setText(QString::number(nim));
        ui->total_dec->setText(QString::number(totalintshot));
        ui->total_dec_2->setText(QString::number(totaldecshot));
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,Tiro,new QTableWidgetItem(QString::number(nim)));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,Inte,new QTableWidgetItem(QString::number(intshot)));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,Dec,new QTableWidgetItem(QString::number(decshot)));


        //Código para fazer zoom onde houve o disparo.
        if(timezoom==0){
            ui->Target->setTransform(QTransform::fromScale(3,3));
            ui->Target->centerOn(QPointF(x,y));
        }

        nim=nim+1;
    }
}

//Código para alternar o ponto entre vermelho e azul enquanto está a fazer zoom onde houve o disparo. Também termina o zoom após algum tempo.
void PistolWindow::shootzoom()
{   
    QPixmap RedDot(":/resources/img/Red Dot.png");
    QPixmap BlueDot(":/resources/img/Blue Dot.png");
    timezoom++;
    if(timezoom%2==0){
        item->setPixmap(RedDot);
    }
    if(timezoom%2==1){
        item->setPixmap(BlueDot);
    }
    if(timezoom==8){
        timedzoom.stop();
        if (zoom==0){
            ui->Target->fitInView(scene->sceneRect(),Qt::KeepAspectRatio); 
        }
        timezoom=0;
    }
}

void PistolWindow::deleteShots()
{
    qDeleteAll(scene->items());
    QImage PistolTarget(":/resources/img/PistolTarget.png");
    scene->addPixmap(QPixmap::fromImage(PistolTarget));
    ui->Target->setScene(scene);
}


void PistolWindow::limparTabela(){
    ui->tableWidget->clear();                    //Apaga dados da tabela
    ui->tableWidget->setRowCount(0);             //Apaga as linhas da tabela
    ui->tableWidget->setHorizontalHeaderLabels(titulos);
}
