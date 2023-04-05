#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <ui_loginwindow.h>
#include <QDir>
#include <QCoreApplication>
#include "loginwindow.h"

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:

private:
    Ui::LoginWindow *ui;
    LoginWindow *loginwindow;
};
#endif // LOGINWINDOW_H