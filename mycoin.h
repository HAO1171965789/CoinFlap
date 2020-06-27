#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>
class MyCoin : public QPushButton
{
    Q_OBJECT
public:
//    explicit MyCoin(QWidget *parent = nullptr);

    //参数表示传入的金币路径 还是银币路径
    MyCoin(QString btnImg);

    //币的属性
    int posX;
    int posY;
    bool flag; //代表正反

    //改变标志的方法
    void changeFlag();

    QTimer *timer1; //正面翻反面
    QTimer *timer2; //反之
    int min = 1;
    int max = 8;

    bool isAnimation = false; //判断是否正在翻转中

    void mousePressEvent(QMouseEvent *e);//重写按下事件

    //是否胜利
    bool isWin = false; //C++里变量未初始化的默认值要看变量的存储类型。一般来说du局部变量未初始化，其默认zhi值不确定，全局变量未初dao始化时，默认值一般为为。



signals:

};

#endif // MYCOIN_H
