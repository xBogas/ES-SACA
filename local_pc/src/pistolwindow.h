#ifndef PISTOLWINDOW_H
#define PISTOLWINDOW_H

#include <QMainWindow>
#include "ui_pistolwindow.h"
#include <QDir>
#include <QCoreApplication>
#include <QTimer>

namespace Ui {
class PistolWindow;
}

class PistolWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PistolWindow(QWidget *parent = nullptr);
    ~PistolWindow();

private slots:
    void on_StartButton_clicked();
    void on_PracticeButton_clicked();
    void on_MatchButton_clicked();
    void on_FinalButton_clicked();
    void processar();


private:
    Ui::PistolWindow *ui;
    short int horas;
    short int minutos;
    short int segundos;
    short int procss;
    QTimer reloj;
};

#endif // PISTOLWINDOW_H
