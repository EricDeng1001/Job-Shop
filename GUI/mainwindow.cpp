#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gantt.h"
#include "../Algorithm/index.h"
#include "../CLI/scanFormatInput.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <cstdio>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gantt(nullptr),
    theResult(nullptr),
    theProblem(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete this->ui;
    delete this->gantt;
    delete this->theProblem;
    delete this->theResult;
}

void MainWindow::on_pushButton_clicked() {

    if (this->gantt) {
        delete this->gantt;
    }
    this->gantt = new Gantt(this,2,2,1,1000);
    this->gantt->addItem(1,1,1,1,50);
    this->gantt->addItem(2,1,2,100,150);
    this->gantt->addItem(1,2,1,50,100);
    this->gantt->addItem(2,2,2,150,200);
    this->gantt->time = 999;
    ui->scrollArea->setWidget(gantt);

}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    this->gantt->time = position;
    this->gantt->repaint();
}

bool MainWindow::getFileName() {
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open input.txt"),
        "/home",
        tr("TXT Files (*.txt)")
    );

    if (filename.isNull()) {
        return false;
    }

    freopen(filename.toStdString().c_str(), "r", stdin);

    return true;
}

void MainWindow::on_openFileBtn_clicked() {
    bool succuess = this->getFileName();
    if (succuess) {
        QMessageBox::information(this, "open succuess", "successfully opened the file");
    }
}
