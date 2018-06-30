// 作者:谭典
#include "gantt.h"
#include <QPen>
#include <QPainter>
#include <QBrush>
#include <QtMath>
#include <QStringList>
#include <QByteArray>
#include <QMouseEvent>

// 不同工序的颜色表
const static QColor COLOR[17] = {
  QColor("#AA311B92"),
  QColor("#AA1A237E"),
  QColor("#AA0D47A1"),
  QColor("#AAB71C1C"),
  QColor("#AA880E4F"),
  QColor("#AA33691E"),
  QColor("#AA827717"),
  QColor("#AA4A148C"),
  QColor("#AA01579B"),
  QColor("#AA006064"),
  QColor("#AA004D40"),
  QColor("#AA1B5E20"),
  QColor("#AAF57F17"),
  QColor("#AAFF6F00"),
  QColor("#AAE65100"),
  QColor("#AABF360C"),
  QColor("#AA3E2723")
};

// 加工列表宽度
#define LINE_HEIGHT (20)

// 创建甘特图
// 参数列表( 工件总数, 机器总数, 缩放比例, 加工时间 )
Gantt::Gantt(QWidget *parent,int product_count,int machine_count,double scale,int maxTime) :
  QWidget(parent)
{
  this->product_count = product_count;
  this->machine_count = machine_count;
  this->maxTime = maxTime;
  this->rescale(scale);
}

//
void Gantt::addItem(int machine, int product, int piece, int startTime, int endTime){
  this->items.append({ machine, product, piece, startTime, endTime });
  items_sorted  = false;
}

void Gantt::clearItems(){
  this->items.clear();
  items_sorted = false;
}

void Gantt::setTime(int time){
  this->time = time;
}

void Gantt::rescale(double scale){
  this->scale = scale;
  this->resize(qFloor(this->maxTime * scale), LINE_HEIGHT*(machine_count+1));
}

void Gantt::play(int speed){
  this->speed = speed;
  if(this->time==0)this->time = 1;
  this->timer.setInterval(speed);
  this->timer.start();
}

void Gantt::stop(){
  this->timer.stop();
  this->time = 0;
  this->speed = 0;
}

void Gantt::pause(){
  this->timer.stop();
}

int Gantt::getTime(){
  return this->time;
}
void Gantt::timerTimeout(){

}

static bool itemLessThan(QList<int> &v1,QList<int> &v2){
  return v1[3] < v2[3];
}

void Gantt::mousePressEvent(QMouseEvent *ev){
  int x = ev->x(),
    y = ev->y();
  int target_machine = y / LINE_HEIGHT;
  // 遍历所有工序,确定要点亮的方块
  for (QList<int> item : this->items){
    int machine = item[0],
      product = item[1],
      startTime = item[3],
      endTime = item[4]
    ;
    int x0 = qFloor(startTime * this->scale);
    int x1 = qFloor(endTime * this->scale);
    if (x>= x0 && x<= x1 && target_machine == machine){
      this->currentSelectedItem = product;
      this->repaint();
      return;
    }
  }
}

void Gantt::paintEvent(QPaintEvent *event){
  if (!this->items_sorted){
    qSort(this->items.begin(),this->items.end(),itemLessThan);
    this->items_sorted = true;
  }

  QPainter painter(this);
  QPen edgePen(Qt::black);
  edgePen.setWidth(3);
  painter.setPen(QColor(0,0,0,255));

  painter.setBackground(QBrush(Qt::white));

  //遍历所有工序,重绘整个表
  for (QList<int> item : this->items){

    int machine = item[0],product = item[1], piece = item[2],startTime = item[3],endTime = item[4];
    if(endTime > this->time) continue;
    int x0 =  qFloor(startTime * this->scale);
    int x1 =  qFloor(endTime * this->scale);
    int y = machine * LINE_HEIGHT;
    //设置颜色
    painter.setBrush(QBrush(COLOR[(product-1) % 17]));
    QRect rect;
    if( product == this->currentSelectedItem ){
      // 画出矩形
      rect = QRect( x0 + 1, y + 1, x1 - x0 - 2, LINE_HEIGHT - 2 );
      painter.setPen(edgePen);
    } else {
      rect = QRect( x0 , y , x1 - x0 , LINE_HEIGHT );
    }
    painter.drawRect(rect);
    painter.setPen( QColor("#eeeeee") );
    painter.drawText(rect,Qt::AlignCenter,QString("%1").arg(product));
    painter.setPen( QColor(0,0,0,0) );
  }
}
