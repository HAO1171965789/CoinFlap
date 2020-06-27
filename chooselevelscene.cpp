#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QSound>


ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    //配置选择关卡场景
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("选择关卡场景");
    //创建菜单栏
    QMenuBar * bar = menuBar();
    setMenuBar(bar);
    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");
    //创建退出的菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //准备开始按钮音效
    QSound * chooseSound = new QSound(":/res/TapButtonSound.wav",this);
    //准备返回按钮音效
    QSound * backSound = new QSound(":/res/BackButtonSound.wav",this);

    //点击退出 实现功能
    connect(quitAction,&QAction::triggered,this,[=](){
        this->close();
    });


    //返回按钮 back
    MyPushButton * backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(),this->height()-backBtn->height());
    // 点击返回 动画效果
    connect(backBtn,&MyPushButton::clicked,[=](){
        qDebug() << "点击";
        //告诉主场景 我返回了 主场景监听ChooseLevelScene的返回按钮

        //播放音效
        backSound->play();

        //延时返回
        QTimer::singleShot(500,this,[=](){
            emit this->chooseBackScene();
            });
        });

    //创建选择关卡的按钮
    for (int i = 0; i<20 ; i++)
    {
        MyPushButton * menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move( 25 + i%4 * 70,150+i/4*70);

        //给按钮添加文字
        //按钮不规则时，下面这种常规操作效果不太好
//        menuBtn->setText(QString::number(i+1));
        QLabel * label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(25 + i%4 * 70,150+i/4*70);
        //设置文字对齐方式 水平和垂直居中
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        //设置让鼠标穿透 51号属性
        label->setAttribute(Qt::WA_TransparentForMouseEvents);

        //监听每个按钮的点击事件
        connect(menuBtn,&MyPushButton::clicked,[=](){
            //播放音效
            chooseSound->play();


            QString str = QString("您选择的是第 %1 关").arg(i+1);
            qDebug() << str;

            //进入游戏场景
            this->hide();
            play = new PlayScence(i+1); //把第几关穿进去

            //设置游戏场景的初始位置
            play->setGeometry(this->geometry());

            play->show();
            connect(play,&PlayScence::chooseBackScene,[=](){
                this->setGeometry(play->geometry());
                this->show();
                delete play;
                play = NULL;

            });

        });


    }



}
//重写绘图事件
void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //画背景上的图标
    pix.load(":/res/Title.png");
    painter.drawPixmap((this->width()-pix.width()) * 0.5,30,pix);
}
