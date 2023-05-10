#ifndef PISTOLWINDOW_H
#define PISTOLWINDOW_H

#include <QMainWindow>
#include <ui_pistolwindow.h>
#include <QDir>
#include <QCoreApplication>
#include <QTimer>
#include <iostream>

namespace Ui {
class PistolWindow;
}

class PistolWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void backButtonClicked();
    void backToDecideModeSignal();

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
    
    QTimer reloj;
    QTimer alert;
    bool block;
    enum Columna
    {
        Lugar,Atleta,Inte,Dec
    };
};

#endif // PISTOLWINDOW_H
