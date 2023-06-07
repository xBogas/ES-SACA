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
    void backToDecideModeSignal();
    void backToDecideTypeSignal();
    void backButtonClickedSignal();
    void new_score(int coordenada_x, int coordenada_y, double pontuação);

private slots:
    void on_ExitButton_clicked();
    void on_MainButton_clicked();
    void on_ShootButton_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void processar();
    void alerta();
    void Disparo(int coordenada_x, int coordenada_y, double pontuação);
    void shootzoom();
    void finalButtonClicked();
    void matchButtonClicked();
    void practiceButtonClicked();
    void startButtonClicked();
    void resetTimer();
    void backToDecideMode();
    void backToDecideType();
    void deleteShots();

protected:
    void showEvent(QShowEvent *revent);
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::RifleWindow *ui;
    short int horas;
    short int minutos;
    short int segundos;
    short int procss;
    short int al;
    short int nim;
    short int zoom;
    short int intshot;
    short int totalintshot;
    double decshot;
    float totaldecshot;
    float x;
    float y;
    short int timezoom;
    QTimer reloj;
    QTimer alert;
    QTimer timedzoom;
    QGraphicsScene *scene;
    QGraphicsPixmapItem* item;
    enum Columna
    {
        Tiro,Inte, Dec
    };
};

#endif // RIFLEWINDOW_H
