//Lasitha Weerasinghe
//100577A

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
    void setClient();

    void on_BtnStartCtsRecord_clicked();

    void on_cmbClipLength_currentIndexChanged(const QString &arg1);

    void updateStatusBar();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
