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

signals:    
    void finalButtonClickedSignal();
    void matchButtonClickedSignal();
    void practiceButtonClickedSignal();
    void startButtonClickedSignal();

private slots:
    void on_ExitButton_clicked();
    void on_MainButton_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void processar();
    void alerta();
    void finalButtonClicked();
    void matchButtonClicked();
    void practiceButtonClicked();
    void startButtonClicked();

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
