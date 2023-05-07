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

public:
    explicit PistolWindow(QWidget *parent = nullptr);
    ~PistolWindow();

signals:    
    void finalButtonClickedSignal();
    void matchButtonClickedSignal();
    void practiceButtonClickedSignal();
    void startButtonClickedSignal();

private slots:
    void on_ExitButton_clicked();
    void on_MainButton_clicked();
    void on_ShootButton_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void processar();
    void alerta();
    void shootzoom();
    void finalButtonClicked();
    void matchButtonClicked();
    void practiceButtonClicked();
    void startButtonClicked();

protected:
    void showEvent(QShowEvent *revent);
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::PistolWindow *ui;
    short int horas;
    short int minutos;
    short int segundos;
    short int procss;
    short int al;
    short int nim;
    short int zoom;
    short int intshot;
    short int totalintshot;
    float decshot;
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

#endif // PISTOLWINDOW_H
