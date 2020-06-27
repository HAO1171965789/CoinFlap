#include "mainscene.h"
#include "ui_mainscene.h"
#include "mypushbutton.h"
#include <QDebug>
#include <QTimer>
#include <QSound> //多媒体模块下的 音效头文件


MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    //配置主场景
    //设置固定大小
    setFixedSize(320,588);

    //设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    //设置标题
    setWindowTitle("翻金币的主场景");

    //退出实现
    connect(ui->actionquit,&QAction::triggered,this,[=](){
         this->close();
    });

    //准备开始按钮音效
    QSound * startSound = new QSound(":/res/TapButtonSound.wav",this);

//    startSound->setLoops(-1);//设置循环播放次数 若是-1 代表无线循环
//    startSound->play();

    //开始按钮
    MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width() * 0.5 - startBtn->width() * 0.5,this->height() * 0.7);

    //实例化选择关卡场景
     ChooseLevelScene * choosescence = new ChooseLevelScene;

    //监听选择关卡中的back信号
    connect(choosescence,&ChooseLevelScene::chooseBackScene,this,[=](){
        this->setGeometry(choosescence->geometry());
        choosescence->hide();
        this->show();
    });

    connect(startBtn,&QPushButton::clicked,[=](){
        //播放音效
        startSound->play();

        startBtn->zoom1();
        startBtn->zoom2();
        //延时后进入到选择关卡场景中
        QTimer::singleShot(500,this,[=](){
            //设置chooseScene场景的位置
            choosescence->setGeometry(this->geometry());

            this->hide();//自身隐藏
            //显示新场景
            choosescence->show();

        });


    });

}
//重写paintEvent事件 画背景图
void MainScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //画背景上的图标
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.5,pix.height() * 0.5);//图标太大，缩小点
    painter.drawPixmap(10,30,pix);
}

MainScene::~MainScene()
{
    delete ui;
}

