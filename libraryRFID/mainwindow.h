#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QWidget>
#include"detect.h"
#include"edit.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void showDetectUi();
    void showEditUi();
private:
    Ui::MainWindow *ui;
    detect *detectUi;
    edit *editUi;
    QImage *image;
};

#endif // MAINWINDOW_H
