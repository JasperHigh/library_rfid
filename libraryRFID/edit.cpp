#include "edit.h"
#include "ui_editmenu.h"
#include<QTimer>
#include<QtDebug>
#include<QLayout>
#include<QMessageBox>
//#include<QString>
edit::edit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::edit)
{
    isExist=false;
    ui->setupUi(this);
    connect(ui->butSave,SIGNAL(clicked()),this,SLOT(saveData()));
    connect(ui->butBack,SIGNAL(clicked()),this,SLOT(returnMain()));

    t=new QTimer(this);//定义一个计时器
    connect(t,SIGNAL(timeout()),this,SLOT(Check_Data()));//每次计时器设定的时间间隔到了就触发读串口数据的函数

//    QSqlQuery sql_query(database);
//    if(!sql_query.exec("select * from student")){
//        qDebug()<<"failed to insert"<<sql_query.lastError();
//    }
}

edit::~edit()
{
    delete ui;
}
void edit::returnMain(){
    this->hide();
    emit back();
    serial->close();
    t->stop();
//    delete serial;
    database.close();
    ui->txtCardNum->setText("");
    ui->txtStuName->setText("");
    ui->txtStuNum->setText("");
    ui->txtMajor->setText("");
    ui->txtPic->setText("");
    isExist=false;
}
void edit::saveData(){
    QSqlQuery sql_query(database);
    QString sql_insert;
    if(!isExist)
    {
        sql_insert=QString("insert into student values('%1','%2','%3','%4','%5')")
                .arg(ui->txtCardNum->text()).arg(ui->txtStuName->text()).arg(ui->txtStuNum->text()).arg(ui->txtMajor->text()).arg(ui->txtPic->text());
    }
    else{
        sql_insert=QString("update student set stuName='%1',stuid='%2',major='%3',avatar='%4' where carid='%5'")
                .arg(ui->txtStuName->text()).arg(ui->txtStuNum->text()).arg(ui->txtMajor->text()).arg(ui->txtPic->text()).arg(ui->txtCardNum->text());
    }
//    QString sql_insert=" insert into student values('2713213','hj','1608041113','物联网','12823213')";
    sql_query.prepare(sql_insert);
    if(!sql_query.exec()){
        qDebug()<<"failed to insert"<<sql_query.lastError();
    }
    else{
        qDebug()<<"success";
    }
}
void edit::closeEvent(QCloseEvent *){
    emit back();
    delete serial;
    database.close();
}
void edit::Start_Read(){

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

       t->start(500);//将定时器时间间隔设定为0.5秒

//       bottomBar->showMessage("COM8连接成功",3000);
//       ui->butStart->setEnabled(false);
   }
   else{
       QMessageBox::warning(this,"","COM8连接失败，请重试！",QMessageBox::Ok);
   }
}
void edit::Check_Data(){
    QByteArray array;
    array[0] = 0xff;
    array[1] = 0xff;
    array[2] = 0x0A;
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
            isExist=true;
            ui->txtCardNum->setText(sql_query.value(0).toString());
            ui->txtStuName->setText(sql_query.value(1).toString());
            ui->txtStuNum->setText(sql_query.value(2).toString());
            ui->txtMajor->setText(sql_query.value(3).toString());
            ui->txtPic->setText(sql_query.value(4).toString());
//            ui->txt->setText(sql_query.value(3).toString());

        }
        else{
            ui->txtCardNum->setText(buff.toHex());
        }
        t->stop();
    }
}
