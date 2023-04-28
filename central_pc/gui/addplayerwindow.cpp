#include "addplayerwindow.h"

AddPlayerWindow::AddPlayerWindow(QWidget *parent, Database *db)
    : QMainWindow(parent)
    , ui(new Ui::AddPlayerWindow)
    , database(db)
{
    ui->setupUi(this); 
}

AddPlayerWindow::~AddPlayerWindow(){
    delete ui;

    ui = nullptr;
}

void AddPlayerWindow::on_cancelButton_clicked(){
    this->close();
}

void AddPlayerWindow::on_saveButton_clicked(){
    //obter strings
    string nome = ui->nameText->text().toStdString();
    int licenca = ui->IDText->text().toInt();
    string clube = ui->clubeText->text().toStdString();
    string disciplina = ui->disciplinaText->text().toStdString();
    QString escalao = ui->escalaoText->text();
    string observacoes = ui->obserText->toPlainText().toStdString();

    //guardar na base de dados
    // int a = database->db_INSERT_Athlete(2423, "Pedro Claro", "M", "Português", 22, "ACP");
    bool insert = database->db_INSERT_Athlete(licenca, nome, "M", "Português", 22, clube);
    if(insert)
        QMessageBox::information(this, "Sucesso", "Atleta adicionado com sucesso");
    else
        QMessageBox::critical(this, "Erro", "Atleta já existente");

    this->close();
}

