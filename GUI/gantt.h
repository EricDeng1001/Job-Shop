//作者:邓智豪
#ifndef __GANTT_H__
#define __GANTT_H__

#include <QWidget>
#include <QTimer>

class Gantt : public QWidget
{
    Q_OBJECT
public:
    explicit Gantt(QWidget *parent,int product_count,int machine_count,double scale,int maxTime);
    Gantt(QWidget *parent,QString text,double scale);
    //The broken machine period marked with productId == 0
    void addItem(int machine,int product,int piece,int startTime,int endTime);

    void clearItems();

    void rescale(double scale);

    void play(int speed);

    void setTime(int time);

    int getTime();

    void stop();

    void pause();

    static Gantt loadFromStandardOutput(QString text);

protected:
    void paintEvent(QPaintEvent *event)  override;
    void mousePressEvent(QMouseEvent *ev) override;
private:
    QList<QList<int>> items;
    bool items_sorted = false;
    int currentSelectedItem = 0;
    int time = 0;
    int product_count;
    int machine_count;
    int speed;
    int maxTime;
    double scale;
    QTimer timer;
signals:

public slots:
     void timerTimeout();
};

#endif
