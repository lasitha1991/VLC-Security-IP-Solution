#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_Btnplay_clicked();

    void on_BtnStream_clicked();

    void on_BtnReceive_clicked();

    void on_BtnWebCam_clicked();

    void on_BtnFromLastMinStream_clicked();

    void setClient();

    void on_BtnStartCtsRecord_clicked();

    void on_BtnLiveStream_clicked();

private:
    Ui::MainWindow *ui;
    bool recording;
    bool streaming;
};

#endif // MAINWINDOW_H
