#include "playscence.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QFont>
#include "mycoin.h"
#include "dataconfig.h"
#include <QPropertyAnimation>
#include <QSound>

//PlayScence::PlayScence(QWidget *parent) : QMainWindow(parent)
//{

//}
PlayScence::PlayScence(int levelNum)
{
    this->levelIndex = levelNum;

    //初始化游戏场景
    //设置固定大小
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("翻金币场景");
    //创建菜单栏
    QMenuBar * bar = menuBar();
    setMenuBar(bar);
    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");
    //创建退出的菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //准备返回按钮音效
    QSound * backSound = new QSound(":/res/BackButtonSound.wav",this);
    //准备翻金币音效
    QSound * flipSound = new QSound(":/res/ConFlipSound.wav",this);
    //准备胜利音效
    QSound * winSound = new QSound(":/res/LevelWinSound.wav",this);

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
        qDebug() << "翻金币场景中 点击了返回";
        //告诉主场景 我返回了 主场景监听ChooseLevelScene的返回按钮

        //播放返回按钮音效

        backSound->play();


        //延时返回
        QTimer::singleShot(500,this,[=](){
            emit this->chooseBackScene();
            });
        });

    //显示当前关卡数
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);

    QString str2 = QString("Level is %1").arg(this->levelIndex);
    label->setText(str2);
    //将字体设置到标签控件中
    label->setFont(font);
    //写一个矩形框 相当于设定了位置 相当于 setFixedSize + move
    label->setGeometry(30,this->height() - 50, 150, 50);

    //初始化每个关卡的二维数组
    dataConfig config;
    for(int i = 0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            this->gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }

    //胜利图片显示
    QLabel * winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width()-tmpPix.width())*0.5,-tmpPix.height());



    //显示金币/银币背景图案
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            //绘制背景图片
            QLabel * label = new QLabel;
            QPixmap pix = QPixmap(":/res/BoardNode(1).png");
            label->setGeometry(0,0,pix.width(),pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(57 + i*50, 200+j*50);

            //if判断是金币还是银币
            QString str;
            if(this->gameArray[i][j] == 1)
            {
                //显示金币
                str = ":/res/Coin0001.png";
            }
            else {
                str = ":/res/Coin0008.png";
            }


            //创建金币
            MyCoin * coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59 + i*50, 204+j*50);//或者设置居中

            //给金币属性赋值
            coin->posX = i;
            coin->posY = j;
            coin->flag = this->gameArray[i][j];//1表示正面

            coinBtn[i][j] = coin;//将coin对象放入数组中维护，以便后续使用

            //点击金币 进行翻转
            connect(coin,&MyCoin::clicked,[=](){

                //循环保证点击时，把其他金币按钮禁用
                for (int i = 0; i<4; i++)
                {
                    for(int j = 0; j<4; j++ )
                    {
                        this->coinBtn[i][j]->isWin = true;

                    }
                }
                flipSound->play();
                coin->changeFlag();
                this->gameArray[i][j] = this->gameArray[i][j]== 0 ? 1 : 0;

                //翻转周围硬币
                QTimer::singleShot(200,this,[=](){

                    if(coin->posX + 1 <= 3) //右侧金币翻转条件 延时翻转

                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY] = this->gameArray[coin->posX+1][coin->posY] == 0 ? 1 : 0;

                    }
                    if(coin->posX - 1 >= 0)//左侧
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY] = this->gameArray[coin->posX-1][coin->posY] == 0 ? 1 : 0;
                    }
                    if(coin->posY - 1 >= 0)//上侧
                    {
                        coinBtn[coin->posX][coin->posY - 1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY - 1] = this->gameArray[coin->posX][coin->posY - 1] == 0 ? 1 : 0;
                    }
                    if(coin->posY + 1 <= 3)//下侧
                    {
                        coinBtn[coin->posX][coin->posY + 1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY + 1] = this->gameArray[coin->posX][coin->posY + 1] == 0 ? 1 : 0;
                    }

                    //翻转后再解除禁用
                    for (int i = 0; i<4; i++)
                    {
                        for(int j = 0; j<4; j++ )
                        {
                            this->coinBtn[i][j]->isWin = false;

                        }
                    }

                    this->isWin = true;
                    for (int i = 0; i<4; i++)
                    {
                        for(int j = 0; j<4; j++ )
                        {
                            if(coinBtn[i][j]->flag == false)
                            {
                                this->isWin = false;
                                break;
                            }
                        }
                    }

                    if(this->isWin)
                    {
                        winSound->play();

                        qDebug() << "游戏胜利";

                        //将所有按钮的胜利标志改为true 如果再次点击按钮 通过重写鼠标按函数 实现不做响应
                        for (int i = 0; i<4; i++)
                        {
                            for(int j = 0; j<4; j++)
                            {
                                coinBtn[i][j]->isWin = true;
                            }
                        }
                        //将胜利的图片移动下来
                        QPropertyAnimation * animation = new QPropertyAnimation(winLabel,"geometry");
                        //设置时间间隔
                        animation->setDuration(1000);
                        //设置开始位置
                        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        //设置结束位置
                        animation->setEndValue(QRect(winLabel->x(),winLabel->y()+170,winLabel->width(),winLabel->height()));
                        //设置缓和曲线
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        //执行动画
                        animation->start();
                    }

                });
            });

        }
    }


}

void PlayScence::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //画背景上的图标
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.5,pix.height() * 0.5);//图标太大，缩小点
    painter.drawPixmap(10,30,pix);
}
