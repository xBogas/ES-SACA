#ifndef PISTOLWINDOW_H
#define PISTOLWINDOW_H

#include <QMainWindow>
#include <ui_pistolwindow.h>
#include <QDir>
#include <QCoreApplication>
#include <QTimer>
#include <iostream>
#include <string>
#include <vector>

namespace Ui {
class PistolWindow;
}

class PistolWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void backButtonClicked();
    void backToDecideModeSignal();
    void tabelaLugarSignal(float pontuação, std::string nome, int numtiros);

public:
    explicit PistolWindow(QWidget *parent = nullptr);
    ~PistolWindow();
    bool practiceSignal;
    bool matchSignal;
    bool finalSignal;
    bool startSignal;
    bool switchModeSignal;
    bool backSignal;
    bool canBack;

private slots:
    void on_StartButton_clicked();
    void on_PracticeButton_clicked();
    void on_MatchButton_clicked();
    void on_FinalButton_clicked();
    void on_ExitButton_clicked();
    void on_switchButton_clicked();
    void on_backButton_clicked();
    void processar();
    void alerta();
    void resetTimer();
    void blockDecideMode();
    void tabelalugar(float pontuação, std::string nome, int numtiros);
    void limparTabela();

private:
    Ui::PistolWindow *ui;
    short int horas;
    short int minutos;
    short int segundos;
    short int procss;
    short int al;
    short int lug_1;
    short int lug_2;
    short int lug_3;
    short int lug_4;
    short int lug_5;
    short int lug_6;
    short int lug_7;
    short int lug_8;
    short int lug_9;
    short int lug_10;
    short int lug_igual;

    short int z;
    short int i;
    short int d;
    QString nm;
    QString lug;
    QVector<QString> jogadores;
    QVector<float> pontuações;
    QVector<short int> ntir;
    QFont fnt;
    QStringList titulos;
    
    QTimer reloj;
    QTimer alert;
    bool block;
    enum Columna
    {
        Lugar,Atleta,NumTir,Inte,Dec
    };
};

#endif // PISTOLWINDOW_H
