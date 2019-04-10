#ifndef DETECT_H
#define DETECT_H

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
class detect;
}

class detect : public QWidget
{
    Q_OBJECT
signals:
    void back();
public:
    explicit detect(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *);
    ~detect();
public slots:
    void returnMain();
    void startRead();
    void Check_Data();
private:
    Ui::detect *ui;
//    QStatusBar *bottomBar;
    QSerialPort *serial;
    QImage *image;
    QSqlDatabase database;
    QTimer *t;
    QSerialPort *serial2;
};

#endif // DETECT_H
