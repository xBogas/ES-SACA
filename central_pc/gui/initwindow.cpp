#include "initwindow.h"

InitWindow::InitWindow(QWidget *parent, Database *db)
    : QMainWindow(parent)
    , ui(new Ui::InitWindow)
    , database(db)
{
    ui->setupUi(this);

    ui->exitButton->setIcon(QIcon(":/resources/img/exit.png"));

    //create window
    mainwindow = new MainWindow();
    addplayerwindow = new AddPlayerWindow(nullptr, database);
    exportwindow = new ExportWindow(nullptr, database);

    // Load the image files using the resource path:/resources/img/744px-Muskets.svg.png
    QPixmap owl(":/resources/img/Target owl.png");
    ui->label->setPixmap(owl);

    //conect signal
    connect(mainwindow, SIGNAL(backButtonClicked()), this, SLOT(onBackButtonClicked()));
}

InitWindow::~InitWindow(){
    delete ui;
    delete mainwindow;
    delete addplayerwindow;
    delete exportwindow;

    ui = nullptr;
    mainwindow = nullptr;
    addplayerwindow = nullptr;
    exportwindow = nullptr;
}

MainWindow* InitWindow::getMainWindow(){
    return mainwindow;
}

ExportWindow* InitWindow::getExportWindow(){
    return exportwindow;
}

void InitWindow::on_importButton_clicked(){
    bool verification = database->db_IMPORT("/home/saca/Atletas.csv", "saca");
    if(verification)
        QMessageBox::information(this, "Sucesso", "Atletas importados com sucesso!");
    else
        QMessageBox::critical(this, "Erro", "Atletas importados sem sucesso!");
}

void InitWindow::on_exportCompButton_clicked(){
    exportwindow->move(100,100);
    exportwindow->show();
    exportwindow->fillTextIfExist();
}

void InitWindow::on_exportAthletesButton_clicked(){
    bool verification = database->db_EXPORT_Athletes("saca", "/home/saca");
    if(verification)
        QMessageBox::information(this, "Sucesso", "Atletas exportados com sucesso!");
    else
        QMessageBox::critical(this, "Erro", "Atletas exportados sem sucesso!");
}

void InitWindow::on_exitButton_clicked(){
    this->close();
}

void InitWindow::on_continueButton_clicked(){
    this->hide();

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect availableGeometry = screen->availableGeometry();
    mainwindow->move(availableGeometry.x() + (availableGeometry.width() - mainwindow->width()) / 2, availableGeometry.y() + (availableGeometry.height() - mainwindow->height()) / 2);
    mainwindow->show();
}

void InitWindow::on_addButton_clicked(){
    addplayerwindow->move(100, 100);
    addplayerwindow->show();
}

void InitWindow::onBackButtonClicked(){
    this->show();
}







