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

void ExportWindow::on_exportButton_clicked(){
    local = ui->localText->text().toStdString();
    categoria = ui->disciplinaText->text().toStdString();
    data = ui-> dataText->date().toString("yyyy-MM-dd").toStdString();

    std::string competitionid = database->create_competitionid(local, data, categoria);

    bool comp = database->verify_competitionid(competitionid);
    if(!comp){
        QMessageBox::critical(this, "Erro", "Competição não existente!");
        return;
    } 

    std::cout << "Competition id successfully inserted" << std::endl;

    int playerID = ui->IDText->text().toInt();

    bool exp = database->db_EXPORT_CompetitionResults(playerID, competitionid, "saca", "/home/saca");
    if(!exp){
        QMessageBox::critical(this, "Erro", "Exportado sem sucesso!");
        return;
    }
    else    
        QMessageBox::information(this, "Sucesso", "Exportado com sucesso!");

    this->close();
}

void ExportWindow::fillTextIfExist(){
    //fill text boxes
    ui->localText->setText(QString::fromStdString(local));
    ui->disciplinaText->setText(QString::fromStdString(categoria));

    QDate date = QDate::fromString(QString::fromStdString(data), "yyyy-MM-dd");
    ui->dataText->setDate(date);
}

