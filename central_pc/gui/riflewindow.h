#ifndef RIFLEWINDOW_H
#define RIFLEWINDOW_H

#include <QMainWindow>
#include <ui_pistolwindow.h>
#include <QDir>
#include <QCoreApplication>
#include <QTimer>
#include <iostream>

namespace Ui {
class RifleWindow;
}

class RifleWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RifleWindow(QWidget *parent = nullptr);
    ~RifleWindow();
    bool practiceSignal;
    bool matchSignal;
    bool finalSignal;
    bool startSignal;

private slots:
    void on_StartButton_clicked();
    void on_PracticeButton_clicked();
    void on_MatchButton_clicked();
    void on_FinalButton_clicked();
    void on_ExitButton_clicked();
    void on_MainButton_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void processar();
    void alerta();

private:
    Ui::RifleWindow *ui;
    short int horas;
    short int minutos;
    short int segundos;
    short int procss;
    short int al;
    QTimer reloj;
    QTimer alert;
};

#endif // RIFLEWINDOW_H
