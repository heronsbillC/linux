#ifndef SNAKE_H
#define SNAKE_H

#include <QMainWindow>
#include<qpainter.h>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class Snake; }
QT_END_NAMESPACE

class Snake : public QMainWindow
{
    Q_OBJECT

public:
    Snake(QWidget *parent = nullptr);
    ~Snake();
    //游戏界面
    void paintEvent(QPaintEvent *event);
    //初始化蛇
    void InitSnake();
    QRect CreateFood();//产生食物
    void IsEat();//判断是否吃到了食物
    void IsHit();//判断是否撞到了自己
    void IsWin();//判断是否赢得了游戏


private slots://定义槽函数的方式
    //蛇的移动
    void Snake_update();
    //void Game_start();
    void Game_again();
    void Game_exit();

private:
    Ui::Snake *ui;
    //QRect vSnakeRect;//声明蛇的身体这一对象。蛇的身体是由一个个小矩形组成的。
    QTimer *timer;//计时器事件，只要设置了这个，每隔一段时间会去执行指定的东西
    int nDirection;//蛇的移动方向
    bool blsRun;//用于控制是否开始的变量
    bool blsOver;//用于控制是否结束的变量
    QPushButton *start;
    QPushButton *exit;
    QPushButton *again;
    void keyPressEvent(QKeyEvent *key);//键盘事件
    QVector<QRect> vSnakeRect; //n个小方块组成的vector，代表蛇
    QRect SnakeHead;//蛇头
    QRect SnakeBody;
    QString Display;//显示的内容
    QRect Food;//用来表示食物的变量
    int Score;//得分
    int Speed;//蛇的移动速度
    int createFoodTime;
};
#endif // SNAKE_H
