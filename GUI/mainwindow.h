#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void on_pushButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_openFileBtn_clicked();

    bool getFileName();

    void excute();

    void drawGannt();

    void startAnimation();
private:
    Ui::MainWindow *ui;
    Gantt *gantt;
    DescribeTable *theProblem;
    Result *theResult;
};

#endif // MAINWINDOW_H
