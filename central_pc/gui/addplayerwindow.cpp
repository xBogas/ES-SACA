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
    //verificação dos campos
    if (ui->IDText->text().trimmed().isEmpty() || ui->nameText->text().trimmed().isEmpty()) {
        // Exibe uma mensagem de erro caso um dos campos esteja vazio
        QMessageBox::critical(this, "Erro", "Nome ou licença do atleta não preenchido");
        return;
    }
    
    //obter strings
    int licenca = ui->IDText->text().toInt();
    std::string nome = ui->nameText->text().toStdString();
    std::string clube = ui->clubeText->text().toStdString();
    std::string disciplina = ui->disciplinaText->text().toStdString();
    std::string escalao = ui->escalaoText->text().toStdString();
    std::string nascimento = ui-> birthText->date().toString("yyyy-MM-dd").toStdString();
    std::string pais = ui->paisText->text().toUpper().toStdString();
    std::string observacoes = ui->obserText->toPlainText().toStdString();

    //guardar na base de dados
    bool insert = database->db_INSERT_Athlete(licenca, nome, clube, disciplina, escalao, nascimento, pais, observacoes);
    if(insert)
        QMessageBox::information(this, "Sucesso", "Atleta adicionado com sucesso");
    else{
        QMessageBox::critical(this, "Erro", "Atleta já existente");
        return;
    }

    this->close();
}

