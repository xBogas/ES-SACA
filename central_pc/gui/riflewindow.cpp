#include "riflewindow.h"
#include "ui_riflewindow.h"
#include <QGraphicsPixmapItem>
#include <string>


RifleWindow::RifleWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RifleWindow)
{
    ui->setupUi(this);

    ui->ExitButton->setIcon(QIcon(":/resources/img/exit.png"));

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(10);
    titulos << "Lugar" << "Atleta" << "Num. de Tiros" << "Pont.(Int)" << "Pont.(Dec)";
    ui->tableWidget->setHorizontalHeaderLabels(titulos);
    ui->tableWidget->verticalHeader()->setVisible(0);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView { font-size: 20pt; }");
    fnt.setPointSize(16);

    practiceSignal = false;
    matchSignal = false;
    finalSignal = false;
    startSignal = false;
    switchModeSignal = false;
    backSignal = false;
    block = false;
    canBack = true;

    segundos=0;
    minutos=0;
    horas=0;
    procss=0;
    al=0;
    lug_1=0;
    lug_2=0;
    lug_3=0;
    lug_4=0;
    lug_5=0;
    lug_6=0;
    lug_7=0;
    lug_8=0;
    lug_9=0;
    lug_10=0;
    lug_igual=0;

    connect(&reloj,SIGNAL(timeout()),this,SLOT(processar()));
    connect(&alert,SIGNAL(timeout()),this,SLOT(alerta()));
    QObject::connect(this, &RifleWindow::backToDecideModeSignal, this, &RifleWindow::resetTimer);
    QObject::connect(this, &RifleWindow::tabelaLugarSignal, this, &RifleWindow::tabelalugar);
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
    limparTabela();
}

void RifleWindow::on_backButton_clicked()
{
    if(canBack){
        backSignal = true;
        resetTimer();

        this->close();
        emit backButtonClicked();
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
        switchModeSignal = true;
        blockDecideMode();
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
    ui->seconds->display(segundos);
    ui->minutes->display(minutos);
    ui->hours->display(horas);
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
    ui->seconds->display(segundos);
    ui->minutes->display(minutos);
    ui->hours->display(horas);
    ui->StartButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
    ui->PracticeButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
    ui->MatchButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
    ui->FinalButton->setStyleSheet("QPushButton{background-color: rgb(100, 100, 100)}");
}



void RifleWindow::tabelalugar(float pontuação, std::string nome, int numtiros){
    //Verificação e alteraração dos lugares na tabela. Descomentar e adicionar os dados em falta. Os dados em falta são o valor de tt(total de atletas), o nome dos atletas para por na tabela e as pontuações dos atletas para comparar-se e por na tabela. Fora disso o código deve funcionar. 
    
    int idx=0;
    int cnt=0;
    int zr=0;
    int zz=1;
    nm=QString::fromStdString(nome);

    if(jogadores.size()==0){
      jogadores.append(nm);
      pontuações.append(pontuação);
      ntir.append(numtiros);
    }
    else{
        for(idx=0; idx<jogadores.size(); idx++){
            if(nm.compare(jogadores[idx])==0){
                pontuações[idx]=pontuação;
                ntir[idx]=numtiros;
                zz=1;
                break;
            }
            else{
                zz=0;
            }
        }
    }
    if(zz==0){
        jogadores.append(nm);
        pontuações.append(pontuação);
        ntir.append(numtiros);
    }

    if(jogadores.size()<10){
        for(z=1; z<=jogadores.size(); z++){
            for(i=0;i<jogadores.size();i++){                  //percorrer todos os atletas, tt=total de atletas.
                if(i==lug_1 || i==lug_2 || i==lug_3 || i==lug_4 || i==lug_5 || i==lug_6 || i==lug_7 || i==lug_8 || i==lug_9){
                    continue;                                 //passa para o seguinte i;
                }
                //Verificar se atleta tem a mesma pontuação que o atleta posto linha anterior da tabela.
                if(z==2 && pontuações[i]==pontuações[lug_1]){
                    lug_igual=1;
                }
                if(z==3 && pontuações[i]==pontuações[lug_2]){
                    lug_igual=1;
                }
                if(z==4 && pontuações[i]==pontuações[lug_3]){
                    lug_igual=1;
                }
                if(z==5 && pontuações[i]==pontuações[lug_4]){
                    lug_igual=1;
                }
                if(z==6 && pontuações[i]==pontuações[lug_5]){
                    lug_igual=1;
                }
                if(z==7 && pontuações[i]==pontuações[lug_6]){
                    lug_igual=1;
                }
                if(z==8 && pontuações[i]==pontuações[lug_7]){
                    lug_igual=1;
                }
                if(z==9 && pontuações[i]==pontuações[lug_8]){
                    lug_igual=1;
                }
                if(z==10 && pontuações[i]==pontuações[lug_9]){
                    lug_igual=1;
                }
                if(lug_igual==0){
                    for(d=0; d<jogadores.size(); d++){          //percorrer restantes atletas para comparar pontuação, verificando se há atleta com pontuação maior.
                        if(d==lug_1 || d==lug_2 || d==lug_3 || d==lug_4 || d==lug_5 || d==lug_6 || d==lug_7 || d==lug_8 || d==lug_9 || i==d){
                            continue;
                        }
                        else{
                            if(pontuações[i]<pontuações[d]){
                                i=d;
                                zr=1;
                                break;
                            }
                            else{
                                zr=0;
                            }
                        }
                    }
                }
                cnt=i;
                if(zr==0 || zr==1  || lug_igual==1){
                    if(z==1){
                        lug_1=i;
                        break;
                    }
                    if(z==2){
                        lug_2=i;
                        break;
                    }
                    if(z==3){
                        lug_3=i;
                        break;
                    }
                    if(z==4){
                        lug_4=i;
                        break;
                    }
                    if(z==5){
                        lug_5=i;
                        break;
                    }
                    if(z==6){
                        lug_6=i;
                        break;
                    }
                    if(z==7){
                        lug_7=i;
                        break;
                    }
                    if(z==8){
                        lug_8=i;
                        break;
                    }
                    if(z==9){
                        lug_9=i;
                        break;
                    }
                    if(z==10){
                        lug_10=i;
                        break;
                    }
                }
            }

            if(z==1){
                lug="1º";
            }
            if(z==2){
                if(lug_igual!=1){
                    lug="2º";
                }
            }
            if(z==3){
                if(lug_igual!=1){
                    lug="3º";
                }
            }
            if(z==4){
                if(lug_igual!=1){
                    lug="4º";
                } 
            }
            if(z==5){
                if(lug_igual!=1){
                    lug="5º";
                }
            }
            if(z==6){
                if(lug_igual!=1){
                    lug="6º";
                }
            }
            if(z==7){
                if(lug_igual!=1){
                    lug="7º";
                }
            }
            if(z==8){
                if(lug_igual!=1){
                    lug="8º";
                }
            }
            if(z==9){
                if(lug_igual!=1){
                    lug="9º";
                }
            }
            if(z==10){
                if(lug_igual!=1){
                    lug="10º";
                }
            }
            ui->tableWidget->setItem(z-1,Lugar,new QTableWidgetItem(lug));
            ui->tableWidget->setItem(z-1,Atleta,new QTableWidgetItem(jogadores[cnt]));
            ui->tableWidget->setItem(z-1,NumTir,new QTableWidgetItem(QString::number(ntir[cnt])));
            ui->tableWidget->setItem(z-1,Inte,new QTableWidgetItem(QString::number(static_cast<int>(pontuações[cnt]))));
            ui->tableWidget->setItem(z-1,Dec,new QTableWidgetItem(QString::number(pontuações[cnt])));

            ui->tableWidget->item(z-1,Lugar)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(z-1,Lugar)->setFont(fnt);
            ui->tableWidget->item(z-1,Atleta)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(z-1,Atleta)->setFont(fnt);
            ui->tableWidget->item(z-1,NumTir)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(z-1,NumTir)->setFont(fnt);
            ui->tableWidget->item(z-1,Inte)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(z-1,Inte)->setFont(fnt);
            ui->tableWidget->item(z-1,Dec)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(z-1,Dec)->setFont(fnt);
            cnt=0;
            lug_igual=0;
        }
    }

    if(jogadores.size()>=10){
        for(z=1; z<=10; z++){
            for(i=0;i<jogadores.size();i++){                  //percorrer todos os atletas, tt=total de atletas.
                if(i==lug_1 || i==lug_2 || i==lug_3 || i==lug_4 || i==lug_5 || i==lug_6 || i==lug_7 || i==lug_8 || i==lug_9){
                    continue;                                 //passa para o seguinte i;
                }
                //Verificar se atleta tem a mesma pontuação que o atleta posto linha anterior da tabela.
                if(z==2 && pontuações[i]==pontuações[lug_1]){
                    lug_igual=1;
                }
                if(z==3 && pontuações[i]==pontuações[lug_2]){
                    lug_igual=1;
                }
                if(z==4 && pontuações[i]==pontuações[lug_3]){
                    lug_igual=1;
                }
                if(z==5 && pontuações[i]==pontuações[lug_4]){
                    lug_igual=1;
                }
                if(z==6 && pontuações[i]==pontuações[lug_5]){
                    lug_igual=1;
                }
                if(z==7 && pontuações[i]==pontuações[lug_6]){
                    lug_igual=1;
                }
                if(z==8 && pontuações[i]==pontuações[lug_7]){
                    lug_igual=1;
                }
                if(z==9 && pontuações[i]==pontuações[lug_8]){
                    lug_igual=1;
                }
                if(z==10 && pontuações[i]==pontuações[lug_9]){
                    lug_igual=1;
                }
                if(lug_igual==0){
                    for(d=0; d<jogadores.size(); d++){          //percorrer restantes atletas para comparar pontuação, verificando se há atleta com pontuação maior.
                        if(d==lug_1 || d==lug_2 || d==lug_3 || d==lug_4 || d==lug_5 || d==lug_6 || d==lug_7 || d==lug_8 || d==lug_9 || i==d){
                            continue;
                        }
                        else{
                            if(pontuações[i]<pontuações[d]){
                                i=d;
                                zr=1;
                                break;
                            }
                            else{
                                zr=0;
                            }
                        }
                    }
                }
                cnt=i;
                if(zr==0 || zr==1  || lug_igual==1){
                    if(z==1){
                        lug_1=i;
                        break;
                    }
                    if(z==2){
                        lug_2=i;
                        break;
                    }
                    if(z==3){
                        lug_3=i;
                        break;
                    }
                    if(z==4){
                        lug_4=i;
                        break;
                    }
                    if(z==5){
                        lug_5=i;
                        break;
                    }
                    if(z==6){
                        lug_6=i;
                        break;
                    }
                    if(z==7){
                        lug_7=i;
                        break;
                    }
                    if(z==8){
                        lug_8=i;
                        break;
                    }
                    if(z==9){
                        lug_9=i;
                        break;
                    }
                    if(z==10){
                        lug_10=i;
                        break;
                    }
                }
            }

            if(z==1){
                lug="1º";
            }
            if(z==2){
                if(lug_igual!=1){
                    lug="2º";
                }
            }
            if(z==3){
                if(lug_igual!=1){
                    lug="3º";
                }
            }
            if(z==4){
                if(lug_igual!=1){
                    lug="4º";
                } 
            }
            if(z==5){
                if(lug_igual!=1){
                    lug="5º";
                }
            }
            if(z==6){
                if(lug_igual!=1){
                    lug="6º";
                }
            }
            if(z==7){
                if(lug_igual!=1){
                    lug="7º";
                }
            }
            if(z==8){
                if(lug_igual!=1){
                    lug="8º";
                }
            }
            if(z==9){
                if(lug_igual!=1){
                    lug="9º";
                }
            }
            if(z==10){
                if(lug_igual!=1){
                    lug="10º";
                }
            }
            ui->tableWidget->setItem(z-1,Lugar,new QTableWidgetItem(lug));
            ui->tableWidget->setItem(z-1,Atleta,new QTableWidgetItem(jogadores[cnt]));
            ui->tableWidget->setItem(z-1,NumTir,new QTableWidgetItem(QString::number(ntir[cnt])));
            ui->tableWidget->setItem(z-1,Inte,new QTableWidgetItem(QString::number(static_cast<int>(pontuações[cnt]))));
            ui->tableWidget->setItem(z-1,Dec,new QTableWidgetItem(QString::number(pontuações[cnt])));

            ui->tableWidget->item(z-1,Lugar)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(z-1,Lugar)->setFont(fnt);
            ui->tableWidget->item(z-1,Atleta)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(z-1,Atleta)->setFont(fnt);
            ui->tableWidget->item(z-1,NumTir)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(z-1,NumTir)->setFont(fnt);
            ui->tableWidget->item(z-1,Inte)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(z-1,Inte)->setFont(fnt);
            ui->tableWidget->item(z-1,Dec)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->item(z-1,Dec)->setFont(fnt);
            cnt=0;
            lug_igual=0;
        }
    }

    lug_1=-1;
    lug_2=-1;
    lug_3=-1;
    lug_4=-1;
    lug_5=-1;
    lug_6=-1;
    lug_7=-1;
    lug_8=-1;
    lug_9=-1;
    lug_10=-1;
    
}

void RifleWindow::limparTabela(){
    ui->tableWidget->clear();                    //Apaga dados da tabela 
    ui->tableWidget->setHorizontalHeaderLabels(titulos);
}