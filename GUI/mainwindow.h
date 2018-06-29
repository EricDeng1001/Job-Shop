#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QThread>
#include "gantt.h"
#include "../Algorithm/index.h"

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

    void on_openFileBtn_clicked();

    bool getFileName();

    void drawGannt();

    //void startAnimation();

    void on_selectOutputFIleBtn_clicked();

    void on_processBtb_clicked();

    void on_horizontalScrollBar_sliderMoved(int position);

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_editingFinished();

    void play();
    void on_pushButton_clicked();
    void onFinish();
private:
    Ui::MainWindow *ui;
    Gantt *gantt;
    QString filename;
    QThread* excuter;
    clock_t startTime;
};

#endif // MAINWINDOW_H
