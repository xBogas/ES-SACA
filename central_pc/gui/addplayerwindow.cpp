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
    QString nome = ui->nameText->text();
    QString licenca = ui->IDText->text();
    QString clube = ui->clubeText->text();
    QString disciplina = ui->disciplinaText->text();
    QString escalao = ui->escalaoText->text();
    QString observacoes = ui->obserText->toPlainText();

    //guardar na base de dados
    // std::cout << database->verify_id(23) << std::endl;

    // std::cout << nome.toStdString() << std::endl;
    // std::cout << licenca.toStdString() << std::endl;
    // std::cout << clube.toStdString() << std::endl;
    // std::cout << disciplina.toStdString() << std::endl;
    // std::cout << escalao.toStdString() << std::endl;
    // std::cout << observacoes.toStdString() << std::endl;

    this->close();
}

