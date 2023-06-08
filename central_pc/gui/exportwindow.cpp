#include "exportwindow.h"

ExportWindow::ExportWindow(QWidget *parent, Database *db)
    : QMainWindow(parent)
    , ui(new Ui::ExportWindow)
    , database(db)
{
    ui->setupUi(this); 
}

ExportWindow::~ExportWindow(){
    delete ui;

    ui = nullptr;
}

void ExportWindow::on_cancelButton_clicked(){
    this->close();
}

void ExportWindow::on_saveButton_clicked(){

    this->close();
}

void ExportWindow::fillTextIfExist(){
    //fill text boxes
    ui->localText->setText(QString::fromStdString(local));
    ui->disciplinaText->setText(QString::fromStdString(categoria));

    QDate date = QDate::fromString(QString::fromStdString(data), "yyyy-MM-dd");
    ui->dataText->setDate(date);
}

