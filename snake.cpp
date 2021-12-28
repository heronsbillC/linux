#include "snake.h"
#include "ui_snake.h"
#include<QPainter>
#include<QTimer>
#include<QKeyEvent>
#include <QRandomGenerator>

Snake::Snake(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Snake),
      blsRun(false),//先不开始游戏
      Speed(500)
{
    ui->setupUi(this);
    this->setGeometry(QRect(200,200,290,310));//窗体位置：Qrect表示矩形框，这个矩形框放在距离屏幕左侧600单位，距离屏幕上方300单位，长290，高310.
    timer=new QTimer(this);//设定计时器
    start = new QPushButton;
    start->setParent(this);
    start->setText("start");
    start->resize(40,20);
    start->move(15, 280);
    start->setFocusPolicy(Qt::NoFocus);
    start->show();
    //connect(start, SIGNAL(clicked()), this, SLOT(Game_start())); //SIGNAL:信号 SLOT：槽,对信号和槽的连接

    again = new QPushButton;
    again->setParent(this);
    again->setText("again");
    again->resize(40,20);
    again->move(70, 280);
    again->setFocusPolicy(Qt::NoFocus);
    again->setCheckable(true);
    again->show();
    connect(again, SIGNAL(clicked()), this, SLOT(Game_again())); //SIGNAL:信号 SLOT：槽,对信号和槽的连接


    exit = new QPushButton;
    exit->setParent(this);
    exit->setText("exit");
    exit->resize(40,20);
    exit->move(120, 280);
    exit->setFocusPolicy(Qt::ClickFocus);
    exit->show();
    connect(exit, SIGNAL(clicked()), this, SLOT(Game_exit())); //SIGNAL:信号 SLOT：槽,对信号和槽的连接
}


Snake::~Snake()
{
    delete ui;
}


//实现游戏界面，所有在游戏界面中显示的都要在这个函数中实现，这一函数在第一次启动程序和调用update的时候会被执行
void Snake::paintEvent(QPaintEvent *event){ //所有的绘图都要在paintEvent函数里面进行
    QPainter painter(this);
    if(!blsRun)//只在第一次运行的时候初始化蛇
        InitSnake();    //在游戏界面中出现蛇

    //画游戏的背景，这里先设置外墙的墙体
    painter.setPen(Qt::black);//外边框用黑色的画笔去画
    painter.setBrush(Qt::gray);//设置填充的颜色为灰色
    painter.drawRect(15,15,260,260);//绘制矩形，以外部的框体为基准，要学会换算四个边界的坐标
    painter.setPen(Qt::black);//同理，再设置内墙的墙体
    painter.setBrush(Qt::black);
    painter.drawRect(20,20,250,250);//说明墙的宽度是5个单位

    //画格子，用循环实现
    painter.setPen(Qt::blue);
    for (int i=2;i<=27;i++) {
        painter.drawLine(20,i*10,270,i*10);//画横线
        painter.drawLine(i*10,20,i*10,270);//画竖线
    }

    //显示游戏开始和结束
    QFont font1("Courier",24);//设置字体，颜色
    painter.setFont(font1);
    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);
    painter.drawText(40,150,Display); //将display的内容显示在40，150的位置上

    //显示分数
    QFont font2("Courier",15);
    painter.setFont(font2);
    painter.setPen(Qt::blue);
    painter.setBrush(Qt::red);
    painter.drawText(180,300,"得分：");
    painter.drawText(240,300,QString::number(Score));//把score转换成字符串并显示出来

    //画蛇
    painter.setPen(Qt::black);//设置蛇的边界颜色
    painter.setBrush(Qt::green);//设置蛇的颜色
    painter.drawRects(&vSnakeRect[0],vSnakeRect.size());//把整个蛇画出来

    //在界面中画出食物
    painter.drawPixmap(Food,QPixmap(":/new/img/img/Apple.png"));

    //游戏停止，通过让计时器停止来结束
    if(blsOver)
        timer->stop();
}

//void Snake::Game_start(){
//    InitSnake();
//}


void Snake::Game_again(){
    timer->stop();
    Speed = 1000;
    blsRun = false;
    update();
}


void Snake::Game_exit(){
    QApplication* app;
    app->exit();
}

//用来说明代表蛇的小方块应该画在什么地方
void Snake::InitSnake(){
    //第一次进来的时候显示“游戏开始”
    Display="游戏开始！";
    blsRun=true;//游戏开始了，令成true
    blsOver=false;//游戏没有结束
    nDirection=2;//默认刚开始蛇的移动方向是向下
    Food=CreateFood();//在游戏开始的时候产生食物
    Score=0;//初始化得分=0
    //矩形框
    vSnakeRect.resize(5);//蛇的长度
    //用for循环实现蛇
    for(int i=0;i<vSnakeRect.size();i++){
        QRect rect(100,70+10*i,10,10);
        vSnakeRect[vSnakeRect.size()-1-i]=rect;
    }
    //对计时器的设定
    //timer=new QTimer(this);//设定计时器
    timer->start(Speed);//设定计时器的间隔时间为500ms，用speed表示更统一
    connect(timer,SIGNAL(timeout()),SLOT(Snake_update()));//SIGNAL:信号 SLOT：槽,对信号和槽的连接
}


//键盘事件的写法(固定模式)
void Snake::keyPressEvent(QKeyEvent *event){
    QKeyEvent *key=(QKeyEvent*) event;
    //用switch语句判断键盘输入的是哪个按键
    switch (key->key()) {
    case Qt::Key_Up:nDirection=1;//如果按的是上键，规定蛇的移动方向记为1，也就是说1对应向上
        break;
    //下面三个方向同理
    case Qt::Key_Down:nDirection=2;
        break;
    case Qt::Key_Left:nDirection=3;
        break;
    case Qt::Key_Right:nDirection=4;
        break;
    default:;//默认情况下什么都不做
    }
}


//更新蛇
void Snake::Snake_update(){
    //在蛇出来之后，就不显示“游戏开始”这一提示语句了
    Display="";
    SnakeHead=vSnakeRect.first();//获取蛇头，让蛇头等于vector中的第一个元素
    IsEat();//判断是否吃到食物
    IsHit();//判断是否撞到自己
    IsWin();//判断是否赢了
    //非蛇头的蛇身的移动
    for(int j=0;j<vSnakeRect.size()-1;j++){
        vSnakeRect[vSnakeRect.size()-1-j]=vSnakeRect[vSnakeRect.size()-2-j];
    }
    //根据蛇头的朝向来更新蛇的位置
    switch (nDirection) {
    case 1:
        SnakeHead.setTop(SnakeHead.top()-10);//如果是往上移动，让蛇的上面的坐标减10
        SnakeHead.setBottom(SnakeHead.bottom()-10);//同时蛇的底部也向上移动10
        break;
    //向其他三个方向移动同理
    case 2:
        SnakeHead.setTop(SnakeHead.top()+10);
        SnakeHead.setBottom(SnakeHead.bottom()+10);
        break;
    case 3:
        SnakeHead.setLeft(SnakeHead.left()-10);
        SnakeHead.setRight(SnakeHead.right()-10);
        break;
    case 4:
        SnakeHead.setLeft(SnakeHead.left()+10);
        SnakeHead.setRight(SnakeHead.right()+10);
        break;
    default:;//默认情况下就什么都不做
    }
    vSnakeRect[0]=SnakeHead;
    //如果蛇头离开了墙壁的范围，即蛇头撞墙，则游戏结束
    if(SnakeHead.left()<20||SnakeHead.right()>270||SnakeHead.top()<20||SnakeHead.bottom()>270){
        blsOver = true;
        //显示“游戏结束”
        Display = "游戏结束";
    }
    update();//刷屏，更新
}


//产生食物
QRect Snake::CreateFood(){
    int x,y;
    //随机产生食物，使用到随机函数
    //x和y是0到25之间的随机的整数
    x=QRandomGenerator::global()->generate()%25;
    y=QRandomGenerator::global()->generate()%25;
    QRect rect(20+x*10,20+y*10,10,10);
    return rect;
}


//判断是否吃到了食物
void Snake::IsEat(){
    //吃到食物，蛇变长(注意思考怎样才叫做蛇变长一节)其实是下一个刷新的时刻让蛇身变长了，得分加10
    if(SnakeHead==Food){
        SnakeHead=Food;//如果吃到了食物，就将食物所在位置变成新的蛇头
        vSnakeRect.push_back(vSnakeRect.last());//把数组中最后一个元素再重复地添加到数组中，就相当于蛇多了一节
        Food = CreateFood();//再产生新的食物
        Score=Score+10;
        Speed=Speed-60;//每次吃到果子，运行间隔减少60ms
        timer->stop();//停止之前的计时器
        timer->start(Speed);//把更新之后的计时器启动

    }
}


//判断是否撞到自己
void Snake::IsHit(){
    //用for循环遍历蛇身，0是蛇头，因此从1开始
    for(int i=1;i<vSnakeRect.size();i++){
        //如果撞上了
        if(SnakeHead==vSnakeRect[i]){
            Display="游戏结束";
            blsOver=true;
            update();//刷新界面，出现"游戏结束"提示语
        }
    }
}


//判断是否游戏胜利
void Snake::IsWin(){
    //设置如果达到了100分则游戏胜利
    if(Score==50){
        Display="恭喜你赢了";
        blsOver=true;
        update();
    }
}
