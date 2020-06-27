#ifndef PLAYSCENCE_H
#define PLAYSCENCE_H

#include <QMainWindow>
#include "mycoin.h"

class PlayScence : public QMainWindow
{
    Q_OBJECT
public:
//    explicit PlayScence(QWidget *parent = nullptr);
    PlayScence(int levelNum);
    int levelIndex;//内部成员函数 记录关卡数

    //重写paintEvent事件
    void paintEvent(QPaintEvent *);

    int gameArray[4][4];//写个二维数组 维护每个关卡的具体数据

    MyCoin * coinBtn[4][4];

    //是否胜利
    bool isWin;
signals:
    void chooseBackScene();
};

#endif // PLAYSCENCE_H
