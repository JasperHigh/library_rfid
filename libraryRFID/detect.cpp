#include "detect.h"
#include "ui_detect.h"
#include<QTimer>
#include<QtDebug>
#include<QAction>
#include<QLayout>
#include<QMessageBox>
//#include<QImage>
//#include "ui_editmenu.h"
//#include"mainwindow.h"
detect::detect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::detect)
{
    ui->setupUi(this);
    connect(ui->butBack,SIGNAL(clicked()),this,SLOT(returnMain()));
    connect(ui->butStart,SIGNAL(clicked()),this,SLOT(startRead()));
//    bottomBar=new QStatusBar(this);
//    QVBoxLayout *vlayout=new QVBoxLayout();
//    vlayout->addWidget(bottomBar);
//    vlayout->addSpacing(-200);
//    setLayout(vlayout);
    image=new QImage();
    image->load("./image/3.jpg");
    ui->labPic->setPixmap(QPixmap::fromImage(image->scaled(ui->labPic->width(),ui->labPic->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation)));
//    image->load("D:/Homework/qt-projects/libraryRFID/image/2.png");
    t=new QTimer(this);
    connect(t,SIGNAL(timeout()),this,SLOT(Check_Data()));//每次计时器设定的时间间隔到了就触发读串口数据的函数
    serial2 =new QSerialPort;
    serial2->setPortName("COM6");
    serial2->open(QIODevice::ReadWrite);
    serial2->setBaudRate(9600);
    serial2->setDataBits(QSerialPort::Data8);
    serial2->setParity(QSerialPort::NoParity);
    serial2->setStopBits(QSerialPort::OneStop);
    serial2->setFlowControl(QSerialPort::NoFlowControl);
}

detect::~detect()
{
    delete ui;
}
void detect::returnMain(){
    emit back();
//    this->close();
    this->hide();
    serial->close();
    delete serial;
    database.close();
    t->stop();
    ui->txtStuNum->setText("");
    ui->txtMajor->setText("");
    ui->txtName->setText("");
    image->load("./image/3.jpg");
    ui->labPic->setPixmap(QPixmap::fromImage(image->scaled(ui->labPic->width(),ui->labPic->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    ui->labInfo->setText("检测中...");
}
void detect::closeEvent(QCloseEvent *){
    emit back();
    delete serial;
    database.close();
}
void detect::startRead(){

    //数据库连接
    database=QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("student.db");
    if(!database.open()){
        qDebug()<<"Failed to connect database"<<database.lastError();;
    }


    serial =new QSerialPort;
    serial->setPortName("COM5");
   if (serial->open(QIODevice::ReadWrite))
   {
       serial->setBaudRate(9600);
       serial->setDataBits(QSerialPort::Data8);
       serial->setParity(QSerialPort::NoParity);
       serial->setStopBits(QSerialPort::OneStop);
       serial->setFlowControl(QSerialPort::NoFlowControl);
//       QTimer *t=new QTimer(this);//定义一个计时器

       t->start(1000);//将定时器时间间隔设定为0.5秒
//       bottomBar->showMessage("COM8连接成功",3000);
       ui->butStart->setEnabled(false);
   }
   else{
//       bottomBar->showMessage("COM8连接失败，请重试！",3000);
       QMessageBox::warning(this,"","COM8连接失败，请重试！",QMessageBox::Ok);
   }
}
void detect::Check_Data(){
    QByteArray array;
    array[0] = char(0xff);
    array[1] = char(0xff);
    array[2] = char(0x0a);
//    array[3] = 0x32;
    serial->write(array);
    serial->waitForReadyRead(300);
    QByteArray buff=serial->readAll();
//    qDebug()<<buff.toHex();

//  探测出卡后开始查询
    if(!buff.toHex().contains("ff")&&buff.toHex().length()==10)
    {
        QSqlQuery sql_query(database);
        QString sql_insert=QString(" select * from student where carid='%1'").arg(QString(buff.toHex()));
        sql_query.prepare(sql_insert);
        sql_query.exec();
        if(sql_query.next()){
            ui->txtStuNum->setText(sql_query.value(1).toString());
            ui->txtMajor->setText(sql_query.value(2).toString());
            ui->txtName->setText(sql_query.value(3).toString());
            image->load(sql_query.value(4).toString());
            qDebug()<<sql_query.value(4).toString();
            ui->labPic->setPixmap(
                        QPixmap::fromImage(image->scaled(ui->labPic->width(),ui->labPic->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation)));
            ui->labInfo->setText("欢迎光临！");
            //门禁控制

            QByteArray array;
            array[0] = 0x0c;
            array[1] = 0x10;
            array[2] = 0x00;
            array[3] = 0x78;
            array[4] = 0x00;
            array[5] = 0x01;
            array[6] = 0x02;
            array[7] = 0x00;
            array[8] = 0x01;
            array[9] = 0x35;
            array[10] = 0x78;

            serial2->write(array);
//            serial2->close();
        }
        else{
            ui->txtStuNum->setText("");
            ui->txtMajor->setText("");
            ui->txtName->setText("");
            image->load("./image/3.jpg");
            ui->labPic->setPixmap(QPixmap::fromImage(image->scaled(ui->labPic->width(),ui->labPic->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation)));

            ui->labInfo->setText("查无此人");
        }
//        qDebug()<<"shit";
    }
//    sql_query.record()
//    if(!sql_query.next()){
//        ui->label->setText("未知卡！");
//    }

//    if(sql_query.record().count()==0){};
}
