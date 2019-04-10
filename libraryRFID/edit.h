#ifndef EDIT_H
#define EDIT_H

#include <QWidget>
#include<QStatusBar>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include<QImage>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include<QTimer>
namespace Ui {
class edit;
}

class edit : public QWidget
{
    Q_OBJECT

public:
    explicit edit(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *);
    ~edit();
signals:
    void back();

public slots:
    void returnMain();
    void saveData();
    void Start_Read();
    void Check_Data();
private:
    Ui::edit *ui;
    QSerialPort *serial;
//    QStatusBar *bottomBar;
    QImage *image;
    QSqlDatabase database;
//    QSqlQuery sql_query;
    QTimer *t;
    bool isExist;
};

#endif // EDIT_H
