#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPixmap>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    this->setWindowTitle("图书门禁系统");
    ui->setupUi(this);
    detectUi=new detect();
    detectUi->hide();
    editUi=new edit();
    editUi->hide();
    connect(ui->butStart,SIGNAL(clicked()),this,SLOT(showDetectUi()));
    connect(ui->butEdit,SIGNAL(clicked()),this,SLOT(showEditUi()));
    connect(ui->butStart,SIGNAL(clicked()),detectUi,SLOT(startRead()));
    connect(ui->butEdit,SIGNAL(clicked()),editUi,SLOT(Start_Read()));
    connect(detectUi,SIGNAL(back()),this,SLOT(show()));
    connect(editUi,SIGNAL(back()),this,SLOT(show()));
    image=new QImage();
    image->load("./image/2.png");

    ui->labPic->setPixmap(QPixmap::fromImage(image->scaled(ui->labPic->width(),ui->labPic->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation)));
//    ui->labPic->setAlignment(Qt::AlignCenter);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEditUi(){
    this->hide();
    editUi->show();

}

void MainWindow::showDetectUi(){
    this->hide();
    detectUi->show();

}
