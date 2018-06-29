#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gantt.h"
#include "../Algorithm/index.h"
#include "../Utils/scanFormatInput.h"
#include "../Utils/formatOutput.h"
#include "../Utils/repair.h"
#include <algorithm>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QThread>
#include <stdio.h>
#include <cstdio>
#include <QFile>

struct RepairCursorNode;

struct RepairCursorNode {
  int cursor;
  int startTime;
  int endTime;
  RepairCursorNode *next;
};

typedef struct {
  RepairCursorNode *head;
  RepairCursorNode *tail;
} RepairCursor;

Result result;
Repair *globalRepairTable = nullptr;
DescribeTable describeTable;

void fillGantt( Gantt* gantt ){
  Formater* formater =
    (Formater*)malloc( sizeof( Formater ) * result.componentCount );//不会超过组件数
  int index;
  for( int i = 0; i < result.machineCount; i++ ){
    index = 0;
    //printf( "M%d", i );
    // 二重循环遍历安排表
    for( int j = 0; j < result.componentCount; j++ ){
      for( int k = 0; k < result.machineCount; k++ ){
        if( result.schedule[j][k].startTime == -1 ){ //第j个工件结束
          break;
        }
        if( result.schedule[j][k].machineIndex == i ){ //第j个工件的第k个工序
          formater[index].startTime = result.schedule[j][k].startTime;
          formater[index].componentIndex = j;
          formater[index].jobIndex = k;
          formater[index].endTime = result.schedule[j][k].endTime;
          index++;
        }
      }
    }

    sortByEndTime( formater, index );

    for( int h = 0; h < index; h++ ){
      gantt->addItem(
                  i,
                  formater[h].componentIndex,
                  formater[h].jobIndex,
                  formater[h].startTime,
                  formater[h].endTime
       );
      //printf(" (%d,%d-%d,%d)",
      //  formater[h].startTime,
      //  formater[h].componentIndex,
      //  formater[h].jobIndex,
      //  formater[h].endTime
      //);
    }
    //printf("\n");
  }

  free( formater );
}

void fillGanttWithRepair( Gantt* gantt, Result result ){
    RepairNode *tmp;
    RepairCursorNode *usedToRecord;
    RepairCursor *printRecord = (RepairCursor*)malloc( sizeof( RepairCursor ) );
    Formater* formater =
      (Formater*)malloc( sizeof( Formater ) * result.componentCount );//不会超过组件数
    int index;
    for( int i = 0; i < result.machineCount; i++ ){
      // record this mahcine's repair block
      printRecord->head = printRecord->tail = (RepairCursorNode*)malloc( sizeof( RepairCursorNode ) );
      index = 0;
      printf( "M%d", i );
      // 二重循环遍历安排表
      for( int j = 0; j < result.componentCount; j++ ){
        for( int k = 0; k < result.machineCount; k++ ){
          if( result.schedule[j][k].startTime == -1 ){ //第j个工件结束
            break;
          }
          if( result.schedule[j][k].machineIndex == i ){ //第j个工件的第k个工序
            formater[index].startTime = result.schedule[j][k].startTime;
            formater[index].componentIndex = j;
            formater[index].jobIndex = k;
            formater[index].endTime = result.schedule[j][k].endTime;
            index++;
          }
        }
      }

      sortByEndTime( formater, index );

      tmp = globalRepairTable->head;
      while( tmp->next ){
          tmp->checked = false;
          tmp = tmp->next;
      }
      tmp = globalRepairTable->head;
      // this loop calc every repair block's index in their machine
      while( tmp->next ){
        if( tmp->machineIndex == i ){ // for this machine
          for( int h = 0; h < index; h++ ){ // modify the arrangement and insert repair block
            if( formater[h].startTime >= tmp->endTime ){
              if( !tmp->checked ){
                usedToRecord = (RepairCursorNode*)malloc( sizeof( RepairCursorNode ) );
                usedToRecord->cursor = h; // original h should be print to repair block
                usedToRecord->next = nullptr;
                usedToRecord->startTime = tmp->startTime;
                usedToRecord->endTime = tmp->endTime;
                printRecord->tail->next = usedToRecord;
                printRecord->tail = usedToRecord;
                tmp->checked = true;
              }
            }
          }
        }
        tmp = tmp->next;
      }

      usedToRecord = printRecord->head->next; // point to first and get ready to itterate

      for( int h = 0; h < index; h++ ){
        if( usedToRecord != nullptr ){
          if( i == usedToRecord->cursor ){
            printf(" (%d,检修,%d)",
              usedToRecord->startTime,
              usedToRecord->endTime
            );
            gantt->addItem(
                        i,
                        -1,//,formater[h].componentIndex,
                        -1,//formater[h].jobIndex,
                        usedToRecord->startTime,
                        usedToRecord->endTime
             );
          }
          usedToRecord = usedToRecord->next;
        }
        gantt->addItem(
                    i,
                    formater[h].componentIndex,
                    formater[h].jobIndex,
                    formater[h].startTime,
                    formater[h].endTime
         );
        //printf(" (%d,%d-%d,%d)",
        //  formater[i].startTime,
        //  formater[i].componentIndex,
        //  formater[i].jobIndex,
        //  formater[i].endTime
        //);
      }
      //printf("\n");
      // TODO: free the printRecord;
    }

    free( formater );
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    excuter(nullptr),
    gantt(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete this->ui;
    delete this->gantt;
}

void MainWindow::drawGannt() {
    QMessageBox::information(this, "process complete", "process complete");
    ui->processingIndicator->setText("process complete");
    if (this->gantt) {
        delete this->gantt;
    }
    this->gantt = new Gantt(
                this,
                result.componentCount,
                result.machineCount,
                1.5,
                result.timeSpan
    );
    fillGantt( this->gantt );
    this->gantt->setTime( 0 );
    QTimer *timer = new QTimer( this );
    connect(timer,SIGNAL(timeout()),this,SLOT(play()));
    timer->start(1000);
    this->startTime = clock();
    ui->scrollArea->setWidget( gantt );
}

bool MainWindow::getFileName() {
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open input.txt"),
        "../",
        tr("TXT Files (*.txt)")
    );

    if (filename.isNull()) {
        return false;
    }

    this->filename = filename;

    return true;
}

void MainWindow::on_openFileBtn_clicked() {
    bool succuess = this->getFileName();
    if (succuess) {
        freopen(this->filename.toStdString().c_str(), "r", stdin);
        QMessageBox::information(this, "successfully opened the input", this->filename.toStdString().c_str());
    }
}

void excute(){
    describeTable = scanFormatInput();

    int beginTime = clock();

    result = solveJSSP( describeTable );

    int endTime = clock();


    printResult( result );
    printf( "Time Used: %.3fs\n", ( endTime - beginTime ) / 1000000.0 );
    printf( "End Time:%d\n", result.timeSpan );
}

void MainWindow::on_selectOutputFIleBtn_clicked() {
    bool succuess = this->getFileName();
    if (succuess) {
        freopen(this->filename.toStdString().c_str(), "w+", stdout);
        QMessageBox::information(this, "successfully opened the output", this->filename.toStdString().c_str());
    }
}

void MainWindow::on_processBtb_clicked() {
    if (this->excuter && this->excuter->isRunning()) {
        return;
    }
    this->excuter = QThread::create( excute );
    connect(this->excuter, SIGNAL(finished()), this, SLOT(drawGannt()));
    this->excuter->start();
    ui->processingIndicator->setText("processing..., please wait");
    QMessageBox::information(this, "processing", "start process, wait for a while...");
}

void MainWindow::on_horizontalScrollBar_sliderMoved(int position) {
    this->gantt->setTime( position );
    this->gantt->repaint();
}

void MainWindow::onFinish(){
    QMessageBox::information(this, "finised", "finised");
    delete this->gantt;
    this->gantt = new Gantt(
                this,
                result.componentCount,
                result.machineCount,
                1,
                result.timeSpan
    );
    freopen("./output2.txt","w+",stdout);
    if( globalRepairTable != nullptr ){
        result = decodeWithRepair( &bestGene, describeTable );
        printResultWithRepair( result );
        fillGanttWithRepair( this->gantt, result );
    } else {
        printResult( result );
    }
    this->gantt->setTime( result.timeSpan );
    ui->scrollArea->setWidget( gantt );
}

void MainWindow::play(){
    if( this->gantt->getTime() >= result.timeSpan ){
        this->onFinish();
        return;
    }
    this->gantt->setTime( this->gantt->getTime() + 12 );
    this->gantt->repaint();
}
void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{

}

void MainWindow::on_lineEdit_editingFinished()
{

}

void MainWindow::on_pushButton_clicked() {
    int machineIndex,
        time;
    int startTime = ( clock() - this->startTime ) / 3 * 1000 * 1000;
    QString text = ui->lineEdit->text();
    sscanf( text.toStdString().c_str(),"C%d %d\n", &machineIndex, &time );
    ui->lineEdit->setText("");
    addRepair( machineIndex, startTime, time );
}
