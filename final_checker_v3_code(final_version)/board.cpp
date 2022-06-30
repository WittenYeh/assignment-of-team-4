#include "board.h"
#include "ask_join_room.h"
#include "qwhtimimg.h"
#include "predefine.h"
#include "showinfo.h"
#include <cmath>
#include <QElapsedTimer>
#include "system.h"
#include "QTimer"
#include <ctime>
#include <iostream>

//定义在user.cpp中
// extern User *curUser;
extern User *me;
// extern User  users[7];
//extern ask_join_room *ask2;
//定义在coord.cpp中
extern Coord center;
extern Coord nullCoord;
extern QString myName;
extern QString myRoomNumber;
extern QString IP;
//extern ShowInfo *showInfo;
// extern Player winnerList[7];

extern System *mysystem;

//Coord coords[25][17];
extern Coord NOTCHOSEN;
//Coord curChosenCoord = NOTCHOSEN;

/* 这是决定是否初始化游戏的变量，整个游戏进程中只需要进行一次，所以采用以下特殊的方式进行设置
 * ywt:2022\4\16 虽然我不知道下面前天的这两行代码具体是写来干嘛的，现在看起来第二个bool变量似乎没什么用
  但是既然程序运行流畅，建议就不要再乱改了......
 * hello出现的时候是false，hello关闭以后设置为true，画完以后再改为false
 */

// board的初始化构造函数
Board::Board(QWidget *parent)
    : QWidget{parent}
{
    // 设置鼠标图标
    QCursor cursor ;
    QPixmap pixmap(MOUSE) ;
    cursor = QCursor(pixmap, -1, -1);
    setCursor(cursor) ;
}

Board::Board(const Board & board)
{

}
// -------------------------------------------鼠标点击相关----------------------------------------------------
// --------------------------------------------更新棋盘信息的函数-----------------------------------------------------------

void deserted()
{
    // 更新棋盘信息的子函数(更新棋子的颜色)(信息设置):包含行棋操作

    // 这里给出警示：不要过度封装函数！！不要封装边沿细节！！不要封装边沿细节！！不要封装边沿细节！！


    // {
    // void Board::updatePieceColor(Coord& beginCoord, Coord& endCoord)
    // {
    //    // 将终点色设置为起点色


    //    endCoord.setColor(beginCoord.getColor());

    //    // 将起点色置为空
    //    beginCoord.setColor(null);

    //    // 将为了这一步而进行的前置操作清空
    //    // 这里的顺序需要注意:应该先调用updateChosenState（因为函数这个里面会设置curChosenCoord的值）
    //    // 再将的指向beginCoord的curChosenCoord置为NULL，才能满足清除可行性标志的条件

    //    // 将起点处的选择状态置为空
    //    this->updateChosenState(beginCoord);

    //    // 撤销原本那个被选中的棋子引发的所有可行性状态都置为0
    //    this->updateFeasibility(curChosenCoord);

    //    // 重新执行painteEvent
    //    // update();
    // }

    // 更新棋盘信息的子函数（更新棋盘的可行棋性）(信息设置)
    // chosenCoord是初始位置
    // int Board::updateFeasibility(Coord *chosenCoord)
    // {
    //    // 如果有被选中的棋子，则更新可行棋性
    //    if (this->curChosenCoord != NULL)
    //    {
    //        //

    //        { //        // 储存当前的搜索结果总数
    //          //        int cur_result = 0;

    //            //        for (int i = 0; i <= 16; i++)
    //            //        {
    //            //            for (int j = 0; j <= 24; j++)
    //            //            {
    //            //                // 当jumpTime为0的时候可以跳到周围的位置
    //            //                if (jumpTime == 0)
    //            //                {
    //            //                    if ((j - 1 >= 0) && (i + 1 <= 16))
    //            //                        if ((coords[j - 1][i +
    // 1].getHaveSlot() ==
    //            // true) &&
    //            //                            (coords[j - 1][i + 1].getColor()
    // ==
    //            //                             null)) coords[j - 1][i +
    //            // 1].setFeasibility(true);

    //            //                    if ((j + 1 <= 24) && (i + 1 <= 16) && (j
    // - 1
    //            // >=
    //            // 0))
    //            //                        if ((coords[j + 1][i +
    // 1].getHaveSlot() ==
    //            //                             true) &&
    //            //                            (coords[j + 1][i + 1].getColor()
    // ==
    //            //                             null)) coords[j - 1][i +
    //            // 1].setFeasibility(true);

    //            //                    if (j - 2 >= 0)
    //            //                        if ((coords[j - 2][i].getHaveSlot()
    // ==
    //            //                             true) &&
    //            //                            (coords[j - 2][i].getColor() ==
    //            //                             null)) coords[j -
    //            // 2][i].setFeasibility(true);

    //            //                    if (j + 2 <= 24)
    //            //                        if ((coords[j + 2][i].getHaveSlot()
    // ==
    //            //                             true) &&
    //            //                            (coords[j + 2][i].getColor() ==
    //            //                             null)) coords[j +
    //            // 2][i].setFeasibility(true);

    //            //                    if ((j - 1 >= 0) && (i - 1 >= 0))
    //            //                        if ((coords[j - 1][i -
    // 1].getHaveSlot() ==
    //            //                             true) &&
    //            //                            (coords[j - 1][i - 1].getColor()
    // ==
    //            //                             null)) coords[j - 1][i -
    //            // 1].setHaveSlot(true);

    //            //                    if ((j + 1 <= 24) && (i - 1 >= 0))
    //            //                        if ((coords[j + 1][i -
    // 1].getHaveSlot() ==
    //            //                             true) &&
    //            //                            (coords[j + 1][i - 1].getColor()
    // ==
    //            //                             null)) coords[j + 1][i -
    //            // 1].setHaveSlot(true);
    //            //                }

    //            //                // 如果jumpTime不为0
    //            //                //
    //            // 如果在当前位置和某个位置之间有一个棋子,并且这条直线上没有其它棋子，并且这个位置是棋槽,并且这个位置没有棋子
    //            //                if ((coords
    //            //                     [(coords[j][i].coord_x +
    //            // chosenCoord->coord_x) /
    //            // 2]
    //            //                     [(coords[j][i].coord_y +
    //            // chosenCoord->coord_y) /
    //            // 2]
    //            //                     .getColor() != null)
    //            //                    && ((coords[j][i].getHaveSlot() == true)
    //            //                        && (coords[j][i].getColor() ==
    // null)))
    //            //                {
    //            //                    cur_result++; // 当前结果总数+1

    //            //                    // 就将这个位置的可行棋性设置为true
    //            //                    coords[j][i].setFeasibility(true);

    //            //                    // 通知调用绘制可行棋标志的函数
    //            //                    //
    //                    connect(&chosenCoord,
    //            //                    //
    //            //                            &Coord::updateFeasibility,
    //            //                    //                            this,
    //            //                    //
    //            //                    //
    //            //                          &Board::updateFeasibilityImag);
    //            //                    //                    emit
    //            // chosenCoord.updateFeasibility();

    //            //                    // 储存进一步进行搜索的结果总数
    //            //                    int next_result =
    //            //                        updateFeasibility(&coords[j][i],
    // jumpTime
    //            // +
    //            // 1);

    //            //                    if (next_result == 0) continue;
    //            //                }
    //            //            }
    //            //        }


    //            // 等全部标记完毕再通知绘图函数
    //            // UPDATE_FEASIBILITY = true;

    //            // 等待深搜算法完成后，重新执行painteEvent
    //            // update();
    //            // return cur_result;
    //        }

    //        //

    //        // remake:

    //        for (int i = 0; i <= 24; i++)
    //        {
    //            for (int j = 0; j <= 16; j++)
    //            {
    //                // 设置边界条件：如果当前状况下
    //            }
    //        }
    //    }

    //    // 如果没有被选中的棋子，则抹除可行棋性
    //    else
    //    {
    //        for (int i = 0; i <= 16; i++)
    //        {
    //            for (int j = 0; j <= 24; j++)
    //            {
    //                coords[j][i].setFeasibility(false);
    //            }
    //        }

    //        // update();
    //        return 0;
    //    }
    // }

    // // 更新棋盘信息的子函数（更新棋盘的选中对象）(信息设置)
    // // 用来设置该棋子是否被选中(如果原本是true,就改成false，如果原本是false，就改成true)
    // void Board::updateChosenState(Coord& chosenCoord)
    // {

    //    // 这里一定要通过重新获取坐标的形式来进行设置，确保设置的是数组中的坐标
    //    // 如果该位置的ChosenState的值为true，则改为false，如果为false，则改为true
    //    coords[chosenCoord.coord_x + 12][chosenCoord.coord_y + 8]
    //    .setChosenState
    //        (!coords[chosenCoord.coord_x + 12][chosenCoord.coord_y + 8]
    //        .getChosenState());

    //    // 如果当前没有被选中的棋子，就让curdChosenCoord指向要被选中的棋子
    //    if (curChosenCoord == NULL) curChosenCoord = &chosenCoord;

    //    // 如果当前有被选中的棋子，就让curdChosenCoord指向空区域
    //    else curChosenCoord = NULL;
    // }
}

void Board::move(Coord& beginCoord,
                 Coord& endCoord)
{
    // qDebug() << "enter move--------------------------------------------------------------------------";

    this->inWhichRoom->coords[endCoord.coord_x + 12][endCoord.coord_y + 8].setColor(this->inWhichRoom->coords[beginCoord.coord_x + 12][beginCoord.coord_y + 8].getColor());
    this->inWhichRoom->coords[beginCoord.coord_x + 12][beginCoord.coord_y + 8].setColor(null);
}

// -------------------------------------------棋盘绘画相关------------------------------------------------------
void Board::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 绘制棋盘布局
    drawBoard(painter);

    // 初始化一局游戏
    if (this->inWhichRoom->INIT_GAME && this->isServer == false)
    {
        this->initGame(painter);
    }

    this->updateChosenStateImag(painter);
    // qDebug() << "enter UpdateChosenStateImag";

    this->updatePieceColorImag(painter);
    // qDebug() << "D6enter UpdateChosenImag";

    this->updateFeasibilityImag(painter);
    // qDebug() << "enter updatePieceColorImag";

}

void initMode() {}

// 初始化一局游戏的所有棋子图像
void Board::initGame(QPainter& painter)
{
    // 重新绘制所有的棋子,并确定所有坐标的目标颜色
    for (int i = 0; i <= 3; i++)
    {
        for (int j = 0; j <= 3 - i; j++)
        {
            // 黄色(一定有)
            painter.setBrush(Qt::yellow);
            painter.drawEllipse(Coord(-3 + i + 2 * j, 5 + i).coord2QPoint(),
                                PIECER,
                                PIECER);
            this->inWhichRoom->coords[-3 + i + 2 * j + 12][5 + i + 8].setColor(yellow);
            // this->inWhichRoom->coords[-3 + i + 2 * j + 12][5 + i + 8].targetColor = yellow;

            // 测试代码
            //            painter.drawText(
            //                coords[-3 + i + 2 * j + 12][5 + i +
            // 8].coord2QPoint(),
            //                "yellow");
            // 蓝色（3人模式没有）
            if ((this->inWhichRoom->userCnt == 6) || (this->inWhichRoom->userCnt == 2))
            {
                painter.setBrush(Qt::blue);
                painter.drawEllipse(Coord(-3  + i + 2 * j, -5 - i).coord2QPoint(),
                                    PIECER,
                                    PIECER);
                this->inWhichRoom->coords[-3  + i + 2 * j + 12][-5 - i + 8].setColor(blue);
                // this->inWhichRoom->coords[-3  + i + 2 * j + 12][-5 - i + 8].targetColor = blue;
            }

            // 测试代码
            //            painter.drawText(
            //                coords[-3  + i + 2 * j + 12][-5 - i +
            // 8].coord2QPoint(),
            //                "blue");
            // 红色(2人模式和3人模式都没有)
            if (this->inWhichRoom->userCnt == 6)
            {
                painter.setBrush(Qt::red);
                painter.drawEllipse(Coord(6 + 2 * i + j, 4 - j).coord2QPoint(),
                                    PIECER,
                                    PIECER);
                this->inWhichRoom->coords[6 + 2 * i + j + 12][4 - j + 8].setColor(red);
                // this->inWhichRoom->coords[6 + 2 * i + j + 12][4 - j + 8].targetColor = red;
            }

            // 测试代码
            //            painter.drawText(coords[6 + 2 * i + j + 12][4 - j +
            // 8].coord2QPoint(),
            //                             "red");
            // 青色(2人模式没有)
            if ((this->inWhichRoom->userCnt == 3) || (this->inWhichRoom->userCnt == 6))
            {
                painter.setBrush(Qt::cyan);
                painter.drawEllipse(Coord(-6 - 2 * i - j, -4 + j).coord2QPoint(),
                                    PIECER,
                                    PIECER);
                this->inWhichRoom->coords[-6 - 2 * i - j + 12][-4 + j + 8].setColor(cyan);
                // this->inWhichRoom->coords[-6 - 2 * i - j + 12][-4 + j + 8].targetColor = cyan;
            }

            // 测试代码
            //            painter.drawText(
            //                coords[-6 - 2 * i - j + 12][-4 + j +
            // 8].coord2QPoint(),
            //                "cyan");
            // 紫色(2人模式没有)
            if ((this->inWhichRoom->userCnt == 3) || (this->inWhichRoom->userCnt == 6))
            {
                painter.setBrush(Qt::magenta);
                painter.drawEllipse(Coord(6 + 2 * i + j,  -4 + j).coord2QPoint(),
                                    PIECER,
                                    PIECER);
                this->inWhichRoom->coords[6 + 2 * i + j + 12][-4 + j + 8].setColor(purple);
                // this->inWhichRoom->coords[6 + 2 * i + j + 12][-4 + j + 8].targetColor = purple;
            }

            // 测试代码
            //            painter.drawText(
            //                coords[6 + 2 * i + j + 12][-4 + j +
            // 8].coord2QPoint(),
            //                "purple");
            // 绿色(2人和3人模式没有)
            if (this->inWhichRoom->userCnt == 6)
            {
                painter.setBrush(Qt::green);
                painter.drawEllipse(Coord(-6 - 2 * i - j, 4 - j).coord2QPoint(),
                                    PIECER,
                                    PIECER);
                this->inWhichRoom->coords[-6 - 2 * i - j + 12][4 - j + 8].setColor(green);
                // this->inWhichRoom->coords[-6 - 2 * i - j + 12][4 - j + 8].targetColor = green;
            }

            // 测试代码
            //            painter.drawText(
            //                coords[-6 - 2 * i - j + 12][4 - j +
            // 8].coord2QPoint(),
            //                "green");
        }
    }

    // 确定所有坐标的目标颜色
    for (int i = 0; i <= 3; i++)
    {
        for (int j = 0; j <= 3 - i; j++)
        {
            this->inWhichRoom->coords[-3 + i + 2 * j + 12][5 + i + 8].targetColor = yellow;
            this->inWhichRoom->coords[-3  + i + 2 * j + 12][-5 - i + 8].targetColor = blue;
            this->inWhichRoom->coords[6 + 2 * i + j + 12][4 - j + 8].targetColor = red;
            this->inWhichRoom->coords[-6 - 2 * i - j + 12][-4 + j + 8].targetColor = cyan;
            this->inWhichRoom->coords[6 + 2 * i + j + 12][-4 + j + 8].targetColor = purple;
            this->inWhichRoom->coords[-6 - 2 * i - j + 12][4 - j + 8].targetColor = green;
        }
    }
    this->inWhichRoom->INIT_GAME = false;
}

// 绘制棋盘布局的函数
void Board::drawBoard(QPainter& painter)
{
    // qDebug() << "enter drawBoard";
    // 设置棋盘的背景--------------------------------------------------------------------
    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::white);

//    painter.setBrush(QBrush(QPixmap(BOARD)));     // 给这个六边形插入背景
//    static const QPoint points1[6] =     // 设置六边形的各个点的坐标
//    {
//        QPoint(366, 10),   QPoint(60,   185),
//        QPoint(60,  505),  QPoint(366,  674),
//        QPoint(672, 505),  QPoint(672,  185)
//    };
//    painter.drawPolygon(points1, 6); // 让画家完成这个六边形的绘画

    // 绘制棋盘上的其它图形区域，并配置以不一样的颜色---------------------------------------------------
    painter.setPen(Qt::black);

    // 设置黄色三角形的三个点
    static const QPoint points2[3] =
    {
        Coord(0, 8).coord2QPoint(), Coord(-3, 5).coord2QPoint(),
        Coord(3, 5).coord2QPoint()
    };
    //设置蓝色三角形的三个点
    static const QPoint points3[3] =
    {
        Coord(0, -8).coord2QPoint(), Coord(-3, -5).coord2QPoint(),
        Coord(3, -5).coord2QPoint()
    };

    // 完成对黄色三角形的绘画
    QColor color1(Qt::yellow);
    color1.setAlpha(ALPHA);
    painter.setBrush(color1);
    painter.drawPolygon(points2, 3);

    //完成对蓝色三角形的绘画
    QColor color2(Qt::blue);
    color2.setAlpha(ALPHA);
    painter.setBrush(color2);
    painter.drawPolygon(points3, 3);

    // 设置红色三角形的三个点
    static const QPoint points4[3] =
    {
        Coord(6, 4).coord2QPoint(), Coord(12, 4).coord2QPoint(),
        Coord(9, 1).coord2QPoint()
    };

    //设置青色三角形的三个点
    static const QPoint points5[3] =
    {
        Coord(-6, -4).coord2QPoint(), Coord(-12, -4).coord2QPoint(),
        Coord(-9, -1).coord2QPoint()
    };

    // 完成对红色三角形的绘画
    QColor color3(Qt::red);
    color3.setAlpha(ALPHA);
    painter.setBrush(color3);
    painter.drawPolygon(points4, 3);

    //完成对青色三角形的绘画
    QColor color4(Qt::cyan);
    color4.setAlpha(ALPHA);
    painter.setBrush(color4);
    painter.drawPolygon(points5, 3);

    // 设置紫色三角形的三个点
    static const QPoint points6[3] =
    {
        Coord(6, -4).coord2QPoint(), Coord(12, -4).coord2QPoint(),
        Coord(9, -1).coord2QPoint()
    };

    //设置绿色三角形的三个点
    static const QPoint points7[3] =
    {
        Coord(-6, 4).coord2QPoint(), Coord(-12, 4).coord2QPoint(),
        Coord(-9, 1).coord2QPoint()
    };

    // 完成对紫色三角形的绘画
    QColor color5(Qt::magenta);
    color5.setAlpha(ALPHA);
    painter.setBrush(color5);
    painter.drawPolygon(points6, 3);

    //完成对绿色三角形的绘画
    QColor color6(Qt::green);
    color6.setAlpha(ALPHA);
    painter.setBrush(color6);
    painter.drawPolygon(points7, 3);

    // 完成对六个白色梯形的绘画
    painter.setBrush(Qt::white);
    static const QPoint points8[4] =
    {
        Coord(-4, 4).coord2QPoint(),  Coord(-3,  5).coord2QPoint(),
        Coord(3,  5).coord2QPoint(),  Coord(4,   4).coord2QPoint()
    };
    static const QPoint points9[4] =
    {
        Coord(4, 4).coord2QPoint(), Coord(6, 4).coord2QPoint(),
        Coord(9, 1).coord2QPoint(), Coord(8, 0).coord2QPoint()
    };
    static const QPoint points10[4] =
    {
        Coord(4, -4).coord2QPoint(), Coord(6, -4).coord2QPoint(),
        Coord(9, -1).coord2QPoint(), Coord(8, 0).coord2QPoint()
    };
    static const QPoint points11[4] =
    {
        Coord(4,  -4).coord2QPoint(),  Coord(3,   -5).coord2QPoint(),
        Coord(-3, -5).coord2QPoint(),  Coord(-4,  -4).coord2QPoint()
    };
    static const QPoint points12[4] =
    {
        Coord(-8, 0).coord2QPoint(),  Coord(-4,  -4).coord2QPoint(),
        Coord(-6, -4).coord2QPoint(), Coord(-9,  -1).coord2QPoint()
    };
    static const QPoint points13[4] =
    {
        Coord(-8, 0).coord2QPoint(), Coord(-4, 4).coord2QPoint(),
        Coord(-6, 4).coord2QPoint(), Coord(-9, 1).coord2QPoint()
    };
    painter.drawPolygon(points8,  4);
    painter.drawPolygon(points9,  4);
    painter.drawPolygon(points10, 4);
    painter.drawPolygon(points11, 4);
    painter.drawPolygon(points12, 4);
    painter.drawPolygon(points13, 4);

    //完成中间部分的绘制(根据角色的不同显示不同效果)
    QPoint points14[6] =
    {
        Coord(-4, 4).coord2QPoint(),
        Coord(-8, 0).coord2QPoint(),
        Coord(-4, -4).coord2QPoint(),
        Coord(4, -4).coord2QPoint(),
        Coord(8, 0).coord2QPoint(),
        Coord(4, 4).coord2QPoint()
    };

    if (this->inWhichRoom->curUser != NULL)
    {
        switch (this->inWhichRoom->curUser->getUserColor())
        {
            case uyellow:
                painter.setBrush(QBrush(QPixmap(YELLOW)));
                break;

            case ured:
                painter.setBrush(QBrush(QPixmap(RED)));
                break;

            case ublue:
                painter.setBrush(QBrush(QPixmap(BLUE)));
                break;

            case ugreen:
                painter.setBrush(QBrush(QPixmap(GREEN)));
                break;

            case ucyan:
                painter.setBrush(QBrush(QPixmap(CYAN)));
                break;

            case upurple:
                painter.setBrush(QBrush(QPixmap(PURPLE)));
                break;

            case unull:
                painter.setBrush(Qt::NoBrush);
        }
    }
    else
    {
        painter.setBrush(Qt::NoBrush);
    }

    painter.drawPolygon(points14, 6);

    // 完成线段的绘画
    QPen pen(Qt::black);

    for (int i = 0; i <= 4; i++)
    {
        // 中间部分水平线
        painter.drawLine(Coord(-8 - i, i).coord2QPoint(),
                         Coord(8 + i, i).coord2QPoint());
        painter.drawLine(Coord(-8 - i, -i).coord2QPoint(),
                         Coord(8 + i, -i).coord2QPoint());
        // 下方三角形水平线
        painter.drawLine(Coord(-4 + i, -4 - i).coord2QPoint(),
                         Coord(4 - i, -4 - i).coord2QPoint());
        // 上方三角形水平线
        painter.drawLine(Coord(-4 + i, 4 + i).coord2QPoint(),
                         Coord(4 - i, 4 + i).coord2QPoint());
        // 中间部分斜线 +x 方向
        painter.drawLine(Coord(-4 - 2 * i, -4).coord2QPoint(),
                         Coord(4 - i, 4 + i).coord2QPoint());
        painter.drawLine(Coord(-4 + i, -4 - i).coord2QPoint(),
                         Coord(4 + 2 * i, 4).coord2QPoint());
        // 中间部分斜线 -x方向
        painter.drawLine(Coord(-4 - 2 * i, 4).coord2QPoint(),
                         Coord(4 - i, -4 - i).coord2QPoint());
        painter.drawLine(Coord(-4 + i, 4 + i).coord2QPoint(),
                         Coord(4 + 2 * i, -4).coord2QPoint());
        // 右下三角形斜线 +x方向
        painter.drawLine(Coord(4 + 2 * i, -4).coord2QPoint(),
                         Coord(8 + i, 0 - i).coord2QPoint());
        // 左下三角形斜线 -x方向
        painter.drawLine(Coord(-4 - 2 * i, -4).coord2QPoint(),
                         Coord(-8 - i, 0 - i).coord2QPoint());
        // 右上三角形斜线 +x方向
        painter.drawLine(Coord(-8 - i, 0 + i).coord2QPoint(),
                         Coord(-4 - 2 * i, 4).coord2QPoint());
        // 左上三角形斜线 -x方向
        painter.drawLine(Coord(4 + 2 * i, 4).coord2QPoint(),
                         Coord(8 + i, 0 + i).coord2QPoint());
    }

    // 完成棋槽的绘制
    // 绘制棋槽要用特殊的线
    painter.setPen(pen);
    // 背景镂空处理
    painter.setBrush(QBrush(QPixmap(BOARD)));

    // 绘制所有的棋槽并设置当前位置是否有棋槽
    for (int i = 0; i <= 4; i++)
    {
        for (int j = -8 - i; j <= 8 + i; j += 2)
        {
            painter.drawEllipse(Coord(j, i).coord2QPoint(),
                                SLOTR, SLOTR);
            painter.drawEllipse(Coord(j, -i).coord2QPoint(),
                                SLOTR, SLOTR);
            this->inWhichRoom->coords[j + 12][i + 8].setHaveSlot(true);
            // 测试代码
            // painter.drawText(coords[j + 12][i + 8].coord2QPoint(),
            //                "slot");
            this->inWhichRoom->coords[j + 12][-i + 8].setHaveSlot(true);
            // 测试代码
            // painter.drawText(coords[j + 12][-i + 8].coord2QPoint(),
            //                "slot");
            // debug += 2;
            // qDebug() << debug;
        }

        for (int j = -i; j <= i; j += 2)
        {
            painter.drawEllipse(Coord(j, 8 - i).coord2QPoint(),  SLOTR,
                                SLOTR);
            painter.drawEllipse(Coord(j, -8 + i).coord2QPoint(), SLOTR,
                                SLOTR);
            this->inWhichRoom->coords[j + 12][8 - i + 8].setHaveSlot(true);
            // 测试代码
            // painter.drawText(coords[j + 12][8 - i + 8].coord2QPoint(),
            //                 "slot");
            this->inWhichRoom->coords[j + 12][-8 + i + 8].setHaveSlot(true);
            // 测试代码
            // painter.drawText(coords[j + 12][-8 + i + 8].coord2QPoint(),
            //                "slot");
            // debug += 2;
            // qDebug() << debug;
        }
    }
}

// 更新棋盘状态的子函数(更新棋子的颜色)
void Board::updatePieceColorImag(QPainter& painter)
{
    // 更新颜色信息,重绘所有棋子
    painter.setPen(Qt::black);
    QColor color;
    color.setAlpha(ALPHA2);

    for (int i = 0; i <= 16; i++)
    {
        for (int j = 0; j <= 24; j++)
        {
            if (this->inWhichRoom->coords[j][i].getHaveSlot() == true)
            {
                switch (this->inWhichRoom->coords[j][i].getColor())
                {
                    case red:
                        color = Qt::red;
                        color.setAlpha(ALPHA2);
                        painter.setBrush(color);
                        painter.drawEllipse(this->inWhichRoom->coords[j][i].coord2QPoint(),
                                            PIECER,
                                            PIECER);
                        break;

                    case yellow:
                        color = Qt::yellow;
                        color.setAlpha(ALPHA2);
                        painter.setBrush(color);
                        painter.drawEllipse(this->inWhichRoom->coords[j][i].coord2QPoint(),
                                            PIECER,
                                            PIECER);
                        break;

                    case blue:
                        color = Qt::blue;
                        color.setAlpha(ALPHA2);
                        painter.setBrush(color);
                        painter.drawEllipse(this->inWhichRoom->coords[j][i].coord2QPoint(),
                                            PIECER,
                                            PIECER);
                        break;

                    case green:
                        color = Qt::green;
                        color.setAlpha(ALPHA2);
                        painter.setBrush(color);
                        painter.drawEllipse(this->inWhichRoom->coords[j][i].coord2QPoint(),
                                            PIECER,
                                            PIECER);
                        break;

                    case cyan:
                        color = Qt::cyan;
                        color.setAlpha(ALPHA2);
                        painter.setBrush(color);
                        painter.setBrush(color);
                        painter.drawEllipse(this->inWhichRoom->coords[j][i].coord2QPoint(),
                                            PIECER,
                                            PIECER);
                        break;

                    case purple:
                        color = Qt::magenta;
                        color.setAlpha(ALPHA2);
                        painter.setBrush(color);
                        painter.drawEllipse(this->inWhichRoom->coords[j][i].coord2QPoint(),
                                            PIECER,
                                            PIECER);
                        break;

                    case null:
                        painter.setBrush(QPixmap(BOARD));
                        painter.drawEllipse(this->inWhichRoom->coords[j][i].coord2QPoint(),
                                            SLOTR,
                                            SLOTR);
                        break;
                }
            }
        }
    }

    //UPDATE_PIECECOLOR = false;
}

// 更新棋盘绘图的子函数（更新棋盘的可执行性）
void Board::updateFeasibilityImag(QPainter& painter)
{
    // 遍历所有的坐标，如果一个坐标具有行棋的可行性，就使用特殊标记进行标识
    for (int i = 0; i <= 16; i++)
    {
        for (int j = 0; j <= 24; j++)
        {
            if (this->inWhichRoom->coords[j][i].getFeasibility() == true)
            {
                // qDebug() << "draw updateFeasibility" << coords[j][i].coord_x << " " << coords[j][i].coord_y;
                // qDebug() << "draw updateFeasibility point" << coords[j][i].coord2QPoint().x() << " " << coords[j][i].coord2QPoint().y();
                painter.setPen(QColor(169, 169, 169));
                painter.setBrush(QColor(169, 169, 169));
                painter.drawEllipse(this->inWhichRoom->coords[j][i].coord2QPoint(),
                                    FEASIBILITYR,
                                    FEASIBILITYR);
            }
        }
    }

    //UPDATE_FEASIBILITY = false;
}

// 更新棋盘绘图的子函数（更新棋盘的选中对象）
void Board::updateChosenStateImag(QPainter& painter)
{
    for (int i = 0; i <= 16; i++)
    {
        for (int j = 0; j <= 24; j++)
        {
            if (this->inWhichRoom->coords[j][i].getChosenState() == true)
            {
                // qDebug() << "update this Coord's Chosen:" <<
                // coords[j][i].coord_x << " " << coords[j][i].coord_y;
                // qDebug() << "draw updateChosenStateImag!";
                painter.setBrush(Qt::NoBrush);
                QPen pen(Qt::black, Qt::DashLine);
                painter.setPen(pen);
                // This F bug spends me an whole afternoon!!!
                painter.drawEllipse(this->inWhichRoom->coords[j][i].coord2QPoint(),
                                    CHOSENSTATER,
                                    CHOSENSTATER);
            }
        }
    }

    //UPDATE_CHOSENSTATE = false;
}

// 更新全部内容：完全更新
void Board::updateBoardImag(QPainter& painter)
{
    this->updateBoardImag(painter);
    this->updateChosenStateImag(painter);
    this->updateFeasibilityImag(painter);
}

// 更新玩家列表图像
void Board::updataPlayerImag()
{
    /*TODO unsolves：
     * 绘制棋盘玩家列表
     */
}

//-----------------------------------------信息发送相关子函数------------------------------------------------

/**
 * @brief setExistedUserName 获取并设置在自己进入房间之前其它已经进入房间的用户的名称
 * 在使用这个函数之前一定要先使用setExistedUserReady函数，目的在于先行获取当前房间中的总人数（包括自己）
 */
void Board::setExistedUserName(QString data1)
{
    for (int i = 1; i <= this->inWhichRoom->userCntInRoom - 1; i++)  //userCntInRoom - 1的原因是发送过来的用户列表没有包括用户自己
    {
        this->inWhichRoom->playerList[i].userName = data1.section(" ", i - 1, i - 1);   //房间里有userCntInRoom个人，data1必然会被空格分成userCntInRoom段
    }
}

/**
 * @brief setExistedUserReady 设置其它用户的准备状态
 * 这里面包含了重要的功能：获取当前的总人数
 */
void Board::setExistedUserReady(QString data2)
{
    //data2中的字符数就是在自己进入房间之前的玩家总数
    //得到当前房间内的玩家总数
    this->inWhichRoom->userCntInRoom += data2.size();

    //获取当前房间里的总人数的准备状态
    for (int i = 1; i <= this->inWhichRoom->userCntInRoom - 1; i++)  //userCntInRoom - 1的原因是发送过来的用户列表没有包括用户自己
    {
        if (data2.mid(i - 1, 1) == "1")
        {
            this->inWhichRoom->playerList[i].isReady = true;
        }
    }
}

void Board::Delay_MSec(unsigned int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}
//-----------------------------------------------------------网络连接相关--------------------------------------------------

/**
 * @brief connected 输出连接成功信息
 */
void Board::connected()
{
    // qDebug() << "successfully connect!";

    mysystem->ask2->show();          //连接成功则直接进入房间

    mysystem->showInfo->showSUCCESSFULLY_CONNECTE();  //显示连接成功的信息，用户点击OK后就关闭
}

/**
 * @brief displayError 输出连接失败信息，并设计一个临时窗口展示给用户
 */
void Board::displayError()
{
    // qDebug() << "oh NO, fail to connect...";
    emit client_receive_ERROR("FAIL_TO_CONNECT", "You can examine your network then click reconnect to try again.");
}

void Board::reConnect()
{
    mysystem->showInfo->showCONNECTING();
    mysystem->socket->hello(IP, 16666);
}

//------------------------------------------信息发送相关-------------------------------------------------------

/**
 * @brief client_send_JOIN_ROOM 用来包装一些加入房间的相关操作
 */
void Board::client_send_JOIN_ROOM()
{
    //data1是用户名，data2是房间号
    QString data1 = myRoomNumber;    //此时me指向的是users[0]
    QString data2 = myName;

    NetworkData data(OPCODE::JOIN_ROOM_OP, data1, data2);

    mysystem->socket->send(data);

    // qDebug() << "send";
}

/**
 * @brief client_send_PLAYER_READY 当收到来自ask_join_room的getReady信号时，触发sendReady请求,发送用户名
 */
void Board::client_send_PLAYER_READY()
{
    //构造一个NetworkData类型的对象temp，data1表示当前玩家的用户名
    QString data1 = myName;
    //data2置空
    QString data2;
    NetworkData temp(OPCODE::PLAYER_READY_OP, data1, data2);

    //向服务器发送temp信息
    mysystem->socket->send(temp);
}

/**
 * @brief client_receive_JOIN_ROOM_REPLY 获取并设置在自己进入房间之前其它已经进入房间的用户的名称和准备状态
 * 封装了下面两个函数setExistedUserName和setExistedUserReady
 * 获取了总人数，获取了自己在玩家列表中的位置（me不再是野指针）
 * 通知重绘ask2并允许ask2中Ready键的点击
 */
void Board::client_receive_JOIN_ROOM_REPLY(NetworkData data)
{
    // 用户自己成功加入游戏，总人数+1
    this->inWhichRoom->userCntInRoom++;

    //调用setExistedUserName之前一定要先调用setExistedUserReady
    setExistedUserReady(data.data2);
    setExistedUserName(data.data1);

    //更改用户的信息
    this->inWhichRoom->playerList[this->inWhichRoom->userCntInRoom].userName = myName;
    this->inWhichRoom->playerList[this->inWhichRoom->userCntInRoom].isReady = false;

    //通知ask_join_room类的对象ask2根据信息重绘表格
    mysystem->ask2->updateTable();

    //加入房间成功，允许玩家点击Ready键，并禁用其它三个键
    emit Ready_is_enabled(true);
    emit forbidButton();
}

void Board::client_receive_JOIN_ROOM(NetworkData data)
{
    //将新进入的玩家计入总人数
    this->inWhichRoom->userCntInRoom++;

    //让这名用户加到用户列表的最后
    this->inWhichRoom->playerList[this->inWhichRoom->userCntInRoom].userName = data.data1;

    this->inWhichRoom->playerList[this->inWhichRoom->userCntInRoom].isReady = false;

    //更新玩家列表
    mysystem->ask2->updateTable();
}

void Board::client_receive_PLAYER_READY(NetworkData data)
{
    for (int i = 1; i <= 6; i++)
    {
        if (this->inWhichRoom->playerList[i].userName == data.data1)    //包括自己
        {
            this->inWhichRoom->playerList[i].isReady = true;
            break;  //因为没有重名者
        }
    }
    mysystem->ask2->updateTable();
}

void Board::client_receive_START_GAME(NetworkData data)
{
    // qDebug() << "client_receive_START_GAME";

    this->inWhichRoom->START_GAME = true;                //鼠标事件生效

    mysystem->ask2->setVisible(false);       //一旦开启游戏就将ask2窗口关闭，以免影响视角

    this->inWhichRoom->userCnt = this->inWhichRoom->userCntInRoom;       //玩家总人数就是当前房间中的人数
    this->inWhichRoom->initUsers();                   //确定了玩家人数就可以开始分配颜色了

    this->inWhichRoom->INIT_GAME = true;              //确定了玩家人数之后就可以开始绘制棋盘了
    this->inWhichRoom->USER_HAS_BEEN_INIT = true;     //nextImag图像也可以开始绘制了
    update();

    // data2是玩家列表，data1是区域列表
    // 玩家对号入座是客户端的安排，这里需要根据data的值来分配区域
    for (int i = 1; i <= this->inWhichRoom->userCntInRoom; i ++)
    {
        if (data.data2.section(" ", i - 1, i - 1) == "A")
        {
            this->inWhichRoom->users[1].userName = data.data1.section(" ", i - 1, i - 1);
            this->inWhichRoom->users[1].zoom = data.data2.section(" ", i - 1, i - 1);
        }

        if (data.data2.section(" ", i - 1, i - 1) == "B")
        {
            this->inWhichRoom->users[2].userName = data.data1.section(" ", i - 1, i - 1);
            this->inWhichRoom->users[2].zoom = data.data2.section(" ", i - 1, i - 1);
        }

        if (data.data2.section(" ", i - 1, i - 1) == "C")
        {
            this->inWhichRoom->users[3].userName = data.data1.section(" ", i - 1, i - 1);
            this->inWhichRoom->users[3].zoom = data.data2.section(" ", i - 1, i - 1);
        }

        if (data.data2.section(" ", i - 1, i - 1) == "D")
        {
            this->inWhichRoom->users[4].userName = data.data1.section(" ", i - 1, i - 1);
            this->inWhichRoom->users[4].zoom = data.data2.section(" ", i - 1, i - 1);
        }

        if (data.data2.section(" ", i - 1, i - 1) == "E")
        {
            this->inWhichRoom->users[5].userName = data.data1.section(" ", i - 1, i - 1);
            this->inWhichRoom->users[5].zoom = data.data2.section(" ", i - 1, i - 1);
        }

        if (data.data2.section(" ", i - 1, i - 1) == "F")
        {
            this->inWhichRoom->users[6].userName = data.data1.section(" ", i - 1, i - 1);
            this->inWhichRoom->users[6].zoom = data.data2.section(" ", i - 1, i - 1);
        }
//          this->inWhichRoom->users[i].zoom = data.data2.section(" ", i - 1, i - 1);
//          this->inWhichRoom->users[i].userName = data.data1.section(" ", i - 1, i - 1);
    }

    for (int i = 1; i <= 6; i++)
    {
        if (this->inWhichRoom->users[i].userName != "")
        {
            // qDebug() << "Users[" << i << "]" << " is " << this->inWhichRoom->users[i].zoom;
        }
    }

    // 初始化me指针
    for (int i = 1; i <= 6; i++)
    {
        if (this->inWhichRoom->users[i].userName == myName)
        {
            me = &this->inWhichRoom->users[i];
        }
    }

    // 设置curUser指针
    this->inWhichRoom->curUser = &this->inWhichRoom->users[6];

    // 通知绘图函数paintEvent在桌面上重绘信息（重绘玩家列表）
    this->update();
}

void Board::client_receive_START_TURN(NetworkData data)
{
    // qDebug() << "client_receive_START_TURN";

    //将倒计时设置为三十秒
    this->inWhichRoom->countdown->setSecs(TIME);
    //呈现倒计时
    this->inWhichRoom->countdown->setVisible(true);
    //倒计时开始
    this->inWhichRoom->countdown->start();
    //在接收到服务端发送的MOVE信号时结束倒计时（设置为10000秒）

    this->inWhichRoom->next();
}

void Board::client_send_MOVE()
{
    // qDebug() << "client_send_MOVE";

    QString data1;
    QString data2;

    data1 = me->zoom;            //设置data1

    data2 = me->track2str();   //设置data2

    // qDebug() << me->userName << "send his move track is" << data2;

    mysystem->socket->send(NetworkData(OPCODE::MOVE_OP, data1, data2));   //发送信息
}

void Board::client_receive_MOVE(NetworkData data)
{
    qDebug() << "client_receive_MOVE";

    this->inWhichRoom->countdown->setVisible(false);   // 设置计时器不可见（关闭计时器）
    this->inWhichRoom->countdown->setSecs(10000);      // 强行设置计时器停止（方法很土）

    // qDebug() << "the moveTrack of " << this->inWhichRoom->curUser->userName << " is" << data.data2;

    // 超时判负
    if (data.data2 == "-1")  //如过data2的内容是-1，说明当前用户超时了，直接将他的所有棋子清空
    {
        // qDebug() << "clear the piece of " << this->inWhichRoom->curUser->userName << " because of timeOut";

        for (int i = 0; i <= 24; i++)
        {
            for (int j = 0; j <= 16; j++)
            {
                if (int(this->inWhichRoom->coords[i][j].getColor()) == int(this->inWhichRoom->curUser->getUserColor()))  //找出所有颜色和当前用户颜色相同的棋子
                {
                    this->inWhichRoom->coords[i][j].setColor(null);    //将颜色和当前用户颜色相同的棋子清空
                }
            }
        }

        //将当前用户的是否负状态设置为是，并且在paintEvent中加入相关的显示流程
        this->inWhichRoom->curUser->isFail = true;

        //如果当前玩家是最后一个玩家，退出超时判负的功能，不再进行next
        int overCnt = 0;
        for (int i = 1; i <= this->inWhichRoom->userCnt; i++)
        {
            if (this->inWhichRoom->users[i].isWin == true || this->inWhichRoom->users[i].isFail == true)
            {
                overCnt++;
            }
        }
        if (overCnt == this->inWhichRoom->userCnt)
        {
            return;    //如果是最后一个人，直接退出,不再
        }

        this->inWhichRoom->next();
        return;
    }

    // 其它玩家的图像更新
    if (me == this->inWhichRoom->curUser) //如果当前用户就是自己，则不必再复现移动步骤(因为我们的客户端已经自己移动了)
    {
        // qDebug() << "hhhh";
        // this->inWhichRoom->next();
        return;
    }
    else
    {
        //在其它玩家的回合，我们不需要关心移动前后的标志设置和撤销的相关设置，只需要move即可

        // qDebug() << "move for " << this->inWhichRoom->curUser->userName;

        Coord* tempTrack = str2track(data.data2);

        //统计跳跃次数
        int tempJumpTime = 0;     //当前玩家的跳跃次数
        int spaceCnt = 0;         //字符串中的空格个数
        for (int i = 0; i <= data.data2.size() - 1; i++)
        {
            if (data.data2.mid(i, 1) == " ")
            {
                spaceCnt++;
            }
        }
        tempJumpTime = (spaceCnt - 1) / 2;

        qDebug() << "tempJumpTime of " << this->inWhichRoom->curUser->userName << tempJumpTime;
        qDebug() << "move for " << this->inWhichRoom->curUser->userName;

        for (int i = 1; i <= tempJumpTime; i++)
        {
            move(tempTrack[i], tempTrack[i + 1]);//1->2,2->3,3->4......
            Delay_MSec(500);
        }

        // 下一个人
        // this->inWhichRoom->next();    // 调用next函数，因为服务端同意了用户发送的请求，在客户端发生了curUser的变化，jumpTime的清零，nextImag图像的变化
    }
}

void Board::client_receive_END_TURN(NetworkData data)
{
    // me->isWin = true;
    this->inWhichRoom->curUser->isWin = true;

    /*TODO unsolved
     * 应该在paintEvent里面添加一个展示玩家胜利信息的绘图流程（可以在玩家列表里面添加）
     */
}

void Board::client_receive_END_GAME(NetworkData data)
{
    mysystem->ask2->showWINNER_LIST(data.data1);   // 显示玩家获胜列表

    mysystem->ask2->show();                        // 展示ask2窗口

    this->close();                       // 这个是一个直接退出程序的简单粗暴的处理方式，要改进！！！
}

void Board::client_send_LEAVE_ROOM()
{
    mysystem->socket->send(NetworkData(OPCODE::LEAVE_ROOM_OP, myName, myRoomNumber));
}

void Board::client_receive_LEAVE_ROOM(NetworkData data)    //client_receive_LEAVE_ROOM采用类链表操作
{
    for (int i = 1; i <= 6; i++)  //重新安排房间中剩下的所有用户的座位向前对齐
    {
        if (this->inWhichRoom->playerList[i].userName == data.data1)   //找出离开了房间的人在playerList中的什么位置
        {
            this->inWhichRoom->playerList[i].userName = "";    //将这名用户的姓名置空
            this->inWhichRoom->playerList[i].isReady = false; //将准备状态设置为未准备

            for (int j = i; j <= this->inWhichRoom->userCntInRoom - 1; j++)  //后一名用户直接顶上去
            {
                this->inWhichRoom->playerList[j].userName = this->inWhichRoom->playerList[j + 1].userName;
                this->inWhichRoom->playerList[j].isReady = this->inWhichRoom->playerList[j + 1].isReady;
            }

            this->inWhichRoom->playerList[this->inWhichRoom->userCntInRoom].userName = "";
            this->inWhichRoom->playerList[this->inWhichRoom->userCntInRoom].isReady = false;
        }
    }

    this->inWhichRoom->userCntInRoom--;   //这行代码一定要写在上面那些代码的后面，因为那些代码中内部的实现是基于原来的userCntInRoom
    mysystem->ask2->updateTable();
}

//服务端函数---------------------------------------------------------------------------------
void Board::server_receive_JOIN_ROOM(QTcpSocket * socket, NetworkData data)
{
    // 注：data1是房间号，data2是用户名
    // 由于用户当前不在任何一个房间中，所以这里的board指的是notInRoom的board，相当于被我们拿过来临时借用了一下

    // 房间号和人名合法性相关检查
    if (data.data1 == "") // 检查房间名是否为空
    {
        server_send_ERROR(socket, "Soory, please enter your room number");
        return;
    }
    if (data.data2 == "")    // 检查人名是否为空
    {
        server_send_ERROR(socket, "Sorry, You are supposed to enter your name");
        return;   //退出函数
    }

    Room* tempRoom = mysystem->introduceRoom(data);  // 将玩家引入一间房间

    if (tempRoom == mysystem->nullRoom)  // 如果被引进的是一个空房间，说明所有房间已满
    {
        server_send_ERROR(socket, "Sorry, there are no vacant rooms");
        tempRoom = nullptr;
        return;
    }
    if (tempRoom == mysystem->fullRoom)  // 如果被引进的是一间满房间，说明房间已满
    {
        server_send_ERROR(socket, "Sorry, the room is full");
        tempRoom = nullptr;
        return;
    }
    if (tempRoom == mysystem->runningRoom)  // 如果被引进的是一间正在运行的房间
    {
        server_send_ERROR(socket, "Sorry, the game in this room is begin");
        tempRoom = nullptr;
        return;
    }
    if (tempRoom == mysystem->sameNameRoom)  // 如果被引进的是一间名字相同的异常房间
    {
        server_send_ERROR(socket, "Sorry, You have the same name as the user in the room");
        tempRoom = nullptr;
        return;
    }

    // 如果检查无误，向所有已经进入房间的玩家发送用户名
    for (int i = 1; i <= tempRoom->userCntInRoom; i++)
    {
        if (tempRoom->playerList[i].userName != "") //只要用户名非空就代表有人(这个判断其实不必要)
        {
            server_send_JOIN_ROOM(tempRoom->playerList[i].socket, data.data2); //data2表示用户名
        }
    }

    // 如果检查无误，向新加入用户发送玩家列表
    QString tempNameList;
    QString tempReadyList;
    for (int i = 1; i <= tempRoom->userCntInRoom ; i++)             //由于playerList采用类链表处理,所用用户位于前userCntInRoom个单位
    {
        tempNameList.append(tempRoom->playerList[i].userName);  //将玩家的名字写入玩家名字列表
        tempNameList.append(" ");                     //在玩家名字的中间加入一个空格
        tempReadyList.append(QString::number(int(tempRoom->playerList[i].isReady)));  //bool->int->QString,将玩家的准备状态写入准备列表
    }
    tempNameList.chop(1);   //最后一个玩家的后面不需要加入空格,故删掉
    server_send_JOIN_ROOM_REPLY(socket, tempNameList, tempReadyList);   //向这名用户发送用户列表

    // 执行在服务端系统中将玩家加入房间
    tempRoom->userCntInRoom++;
    tempRoom->playerList[tempRoom->userCntInRoom].userName = data.data2;   //存入名字，由于playerList采用类链表处理，故最后一个位置就是可以加入的位置
    tempRoom->playerList[tempRoom->userCntInRoom].socket = socket;
}

void Board::server_send_JOIN_ROOM_REPLY(QTcpSocket *socket, QString nameList, QString readyList)
{
    mysystem->server->send(socket, NetworkData(OPCODE::JOIN_ROOM_REPLY_OP, nameList, readyList));
}

void Board::server_send_JOIN_ROOM(QTcpSocket *socket, QString userName)
{
    mysystem->server->send(socket, NetworkData(OPCODE::JOIN_ROOM_OP, userName, ""));
}

void Board::server_receive_PLATER_READY(QTcpSocket * socket, NetworkData data)
{
    // qDebug() << "receive ready from " << data.data1;

    // 设置并向所有用户转发这名用户的准备信息
    for (int i = 1; i <= this->inWhichRoom->userCntInRoom; i++)
    {
        if (this->inWhichRoom->playerList[i].userName == data.data1)   //找出是哪名玩家发出的准备信息
        {
            this->inWhichRoom->playerList[i].isReady = true;     //设置这名玩家已准备

            for (int j = 1; j <= this->inWhichRoom->userCntInRoom; j++)
            {
                server_send_PLAYER_READY(this->inWhichRoom->playerList[j].socket, this->inWhichRoom->playerList[i].userName);  //将玩家的信息发送给所有用户
            }
            break;                            //只有一名玩家，找到之后直接break
        }
    }

    // 如果发现所有用户均已准备，直接发送开始游戏请求，每次收到这一请求就检查一次
    int readycnt = 0;
    for (int i = 1; i <= this->inWhichRoom->userCntInRoom; i++ )   //进行检查
    {
        if (this->inWhichRoom->playerList[i].isReady)
        {
            readycnt++;
        }
    }
    if (readycnt == this->inWhichRoom->userCntInRoom && this->inWhichRoom->userCntInRoom != 1 && this->inWhichRoom->userCntInRoom != 4 && this->inWhichRoom->userCntInRoom != 5)     //如果所有人都已经点击准备
    {
        this->inWhichRoom->userCnt = this->inWhichRoom->userCntInRoom;       //确定游戏人数

        this->inWhichRoom->historyRoom = this->inWhichRoom->buildHistoryRoom();  // 初始化新房间的历史房间

        // 服务端进行座位分配，将玩家对号入座（服务端的座位分配方式是固定的）
        if (this->inWhichRoom->userCnt == 2)
        {
            this->inWhichRoom->users[1].userName = this->inWhichRoom->playerList[1].userName;
            this->inWhichRoom->users[1].socket = this->inWhichRoom->playerList[1].socket;
            this->inWhichRoom->users[4].userName = this->inWhichRoom->playerList[2].userName;
            this->inWhichRoom->users[4].socket = this->inWhichRoom->playerList[2].socket;
        }
        if (this->inWhichRoom->userCnt == 3)
        {
            for (int i = 1; i <= 3; i++)
            {
                this->inWhichRoom->users[2 * i - 1].userName = this->inWhichRoom->playerList[i].userName;
                this->inWhichRoom->users[2 * i - 1].socket = this->inWhichRoom->playerList[i].socket;
            }
        }
        if (this->inWhichRoom->userCnt == 6)
        {
            for (int i = 1; i <= 6; i++)
            {
                this->inWhichRoom->users[i].userName = this->inWhichRoom->playerList[i].userName;
                this->inWhichRoom->users[i].socket = this->inWhichRoom->playerList[i].socket;
            }
        }

        //制作玩家名称列表和区域列表，发送给所有玩家
        QString tempNameList;
        QString tempZoomList;

        //制作玩家的名称列表
        for (int i = 1; i <= this->inWhichRoom->userCntInRoom ; i++)             //由于playerList采用类链表处理,所用用户位于前userCntInRoom个单位
        {
            tempNameList.append(this->inWhichRoom->playerList[i].userName);  //将玩家的名字写入玩家名字列表,玩家列表的制作方法就是按顺序填入
            tempNameList.append(" ");                     //在玩家名字的中间加入一个空格
        }

        tempNameList.chop(1);   //最后一个玩家的后面不需要加入空格,故删掉

        //制作玩家的区域列表
        switch (this->inWhichRoom->userCnt)
        {
            case 2:
                tempZoomList = "A D";
                break;

            case 3:
                tempZoomList = "A C E";
                break;

            case 6:
                tempZoomList = "A B C D E F";
                break;

            default:
                break;
        }

        //将这两个列表发送给所有玩家并通知游戏开始
        for (int i = 1; i <= this->inWhichRoom->userCnt; i++)
        {
            server_send_START_GAME(this->inWhichRoom->playerList[i].socket, tempNameList, tempZoomList);
        }

        //以下是为了让服务端方便呈现画面信息的语块，它们初始化了整个行棋界面
        this->inWhichRoom->initUsers();                   // 确定了玩家人数就可以开始分配颜色,还会将curUser设置为1
        this->inWhichRoom->START_GAME = true;             // 该房间中的游戏开始变量
        this->inWhichRoom->INIT_GAME = true;              // 确定了玩家人数之后就可以开始绘制棋盘
        this->inWhichRoom->USER_HAS_BEEN_INIT = true;     // nextImag图像也可以开始绘制
        this->inWhichRoom->initGame();    // 初始化棋子的颜色和棋子的目标颜色  // Room里面的initGame是客户端专用的initGame

        //告知所有用户第一个玩家应该开始下棋(此时会有curUser == users[1])
        for (int i = 1; i <= this->inWhichRoom->userCnt; i++)
        {
            server_send_START_TURN(this->inWhichRoom->playerList[i].socket, NetworkData(OPCODE::START_TURN_OP, this->inWhichRoom->curUser->zoom, QString::number(time(NULL))));
        }

        //在服务端进行倒计时
        this->inWhichRoom->countdown->setSecs(TIME);//将倒计时设置为三十秒
        this->inWhichRoom->countdown->setVisible(true);//呈现倒计时
        this->inWhichRoom->countdown->start();//倒计时开始
    }
}

void Board::server_send_PLAYER_READY(QTcpSocket * socket, QString userName)
{
    mysystem->server->send(socket, NetworkData(OPCODE::PLAYER_READY_OP, userName, ""));
}

void Board::server_receive_LEAVE_ROOM(QTcpSocket * socket, NetworkData data)
{
    // 通知所有的客户端(包括已经离开的客户端以外)
    // 因为需要通知到所有的客户端，所以这里在移除客户端之前处理
    for (int i = 1; i <= this->inWhichRoom->userCntInRoom; i++)
    {
        if (this->inWhichRoom->playerList[i].socket != nullptr)
        {
            server_send_LEAVE_ROOM(this->inWhichRoom->playerList[i].socket, data.data2);
        }
    }

    //先在服务端系统将这名用户移除玩家列表（采用类链表模式操作）
    for (int i = 1; i <= 6; i++)  //重新安排房间中剩下的所有用户的座位向前对齐
    {
        if (this->inWhichRoom->playerList[i].userName == data.data1)   //找出离开了房间的人在playerList中的什么位置
        {
            this->inWhichRoom->playerList[i].userName = "";    //将这名用户的姓名置空
            this->inWhichRoom->playerList[i].socket = nullptr;    //将这名用户的指针置空
            this->inWhichRoom->playerList[i].isReady = false; //将准备状态设置为未准备

            for (int j = i; j <= this->inWhichRoom->userCntInRoom - 1; j++)  //后一名用户直接顶上去
            {
                this->inWhichRoom->playerList[j].userName = this->inWhichRoom->playerList[j + 1].userName;
                this->inWhichRoom->playerList[j].socket = this->inWhichRoom->playerList[j + 1].socket;
                this->inWhichRoom->playerList[j].isReady = this->inWhichRoom->playerList[j + 1].isReady;
            }

            //列表末端空出来一个位置，直接将它置空
            this->inWhichRoom->playerList[this->inWhichRoom->userCntInRoom].userName = "";
            this->inWhichRoom->playerList[this->inWhichRoom->userCntInRoom].socket = nullptr;
            this->inWhichRoom->playerList[this->inWhichRoom->userCntInRoom].isReady = false;
        }
    }

    this->inWhichRoom->userCntInRoom--;   //这行代码一定要写在上面那些代码的后面，因为那些代码中内部的实现是基于原来的userCntInRoom

    if (this->inWhichRoom->userCntInRoom == 0)   // 如果某个人离开后当前房间的人数为0
    {
        this->inWhichRoom->clearRoom();
    }
}

void Board::server_send_LEAVE_ROOM(QTcpSocket *socket, QString leavingUserName)
{
    mysystem->server->send(socket, NetworkData(OPCODE::LEAVE_ROOM_OP, leavingUserName, ""));
}

void Board::server_send_START_GAME(QTcpSocket *socket, QString nameList, QString zoomList)
{
    mysystem->server->send(socket, NetworkData(OPCODE::START_GAME_OP, nameList, zoomList));
}

void Board::server_send_START_TURN(QTcpSocket *socket, NetworkData data)
{
    mysystem->server->send(socket, data);
}

void Board::server_receive_MOVE(QTcpSocket * socket, NetworkData data)
{
    // 这个函数里面要写大量的检查机制，哎

    // 如果当前不是该玩家的跳棋回合，发送错误后 return
    if (this->inWhichRoom->curUser->socket != socket)
    {
        server_send_ERROR(socket, "Sorry, you are not in your turn");
        return;
    }

    Coord* tempTrack = str2track(data.data2);    //将data2的移动字符串转成坐标数组

    // 统计跳跃次数
    int tempJumpTime = 0;     //当前玩家的跳跃次数
    int spaceCnt = 0;         //字符串中的空格个数
    for (int i = 0; i <= data.data2.size() - 1; i++)
    {
        if (data.data2.mid(i, 1) == " ")
        {
            spaceCnt++;
        }
    }
    tempJumpTime = (spaceCnt - 1) / 2;

    //    qDebug() << "the jumpTime of curUser is " << tempJumpTime;
    if (this->inWhichRoom->MOVE_TRACK_INFO)
    {
        qDebug() << "room" << this->inWhichRoom->roomNumber << ": " << "the moveTrack of curUser is ";
        for (int i = 1; i <= tempJumpTime + 1; i++)
        {
            std::cout << "(" << tempTrack[i].coord_x << "," << tempTrack[i].coord_y << ") ";
        }
        std::cout << std::endl;
    }
    //查看当前跳跃是否存在重复
    for (int i = 1; i <= tempJumpTime + 1 ; i++)
    {
        int repeatTime = -1;//不计入自身

        for (int j = 1; j <= tempJumpTime + 1; j++)
        {
            if (tempTrack[i] == tempTrack[j])
            {
                repeatTime++;
            }
        }
        if (repeatTime != 0)
        {
            server_send_ERROR(socket, "Your jump is invalid");
            qDebug() << this->inWhichRoom->curUser->userName << " jumping is invalid because of jump to same place";
            return;   //如果异常，在发送完信号之后直接return退出
        }
    }

    // 模拟每一步的跳跃,分析这个坐标数组的移动是否异常
    for (int i = 1; i <= tempJumpTime; i++)
    {
        this->inWhichRoom->choose(tempTrack[i]);   //每次选中的是第i颗棋子

        // 检查一下黄色棋子的状态
        //        for (int i = 0; i <= 24; i++)
        //        {
        //            for (int j = 0; j <= 16; j++)
        //            {
        //                if (this->inWhichRoom->coords[i][j].getColor() == yellow)
        //                {
        //                    qDebug() << "yellow: (" << this->inWhichRoom->coords[i][j].coord_x << "," << this->inWhichRoom->coords[i][j].coord_y << ")";
        //                }
        //            }
        //        }

        this->inWhichRoom->search();               //进行搜索

        //检测一下检测的结果（debug）
//        for (int i = 0; i <= 24; i++)
//        {
//            for (int j = 0 ; j <= 16; j++)
//            {
//                qDebug() << "the color of coords[" << i << "," << j << "]" << " is " << int(this->inWhichRoom->coords[i][j].getColor());
//                qDebug() << "the haveslot of coords[" << i << "," << j << "]" << " is " << int(this->inWhichRoom->coords[i][j].getHaveSlot());
//                if (this->inWhichRoom->coords[i][j].getFeasibility() == true)
//                {
//                    qDebug() << "the feasibility coord is (" << this->inWhichRoom->coords[i][j].coord_x << "," << this->inWhichRoom->coords[i][j].coord_y << ")";
//                }
//            }
//        }

        if (this->inWhichRoom->coords[tempTrack[i + 1].coord_x + 12][tempTrack[ i + 1 ].coord_y + 8].getFeasibility() == false) //如果中间有某点不能到达,注意这里的判断条件：应该是转换成原数组进行判断
        {
            server_send_ERROR(socket, "Sorry, your jump is invalid");
            qDebug() << this->inWhichRoom->curUser->userName << " jumping is invalid because one point in your track is invalid";
            qDebug() << "And the point is" << "(" << tempTrack[i + 1].coord_x << "," << tempTrack[i + 1].coord_y << ")";

            this->inWhichRoom->cancelChoose();
            this->inWhichRoom->cancelFeasibility();    //取消选棋和可行性标志，等下一颗棋子重选

            return;   //如果异常，在发送完信号之后直接return退出
        }

        this->inWhichRoom->cancelChoose();
        this->inWhichRoom->cancelFeasibility();    //取消选棋和可行性标志，选择轨迹列表的第二个位置继续判断

        this->inWhichRoom->curUser->jumpTime++;
    }

    //如果上面的检测没有问题，就通知计时器暂停
    this->inWhichRoom->countdown->setVisible(false);   // 设置计时器不可见（关闭计时器）
    this->inWhichRoom->countdown->setSecs(10000);      // 强行设置计时器停止（方法很土）
    this->inWhichRoom->countdown->start();

    //如果上面的检测没有问题，就在服务端更新图形界面
    for (int i = 1; i <= tempJumpTime; i++)
    {
        move(tempTrack[i], tempTrack[i + 1]);//1->2,2->3,3->4......
        Delay_MSec(500);
        update();         //move一次就update一次，理想状态下这里应该设置延迟0.5ms
    }

    //如果上面的检测没有问题，就向所有玩家转发这一信息（不包括发送者）
    for (int i = 1; i <= this->inWhichRoom->userCnt; i++)
    {
        if (this->inWhichRoom->playerList[i].socket == socket)
        {
            continue;
        }
        server_send_MOVE(this->inWhichRoom->playerList[i].socket, data.data1, data.data2);
    }

    //当这名玩家的MOVE请求后发现这名玩家的所有棋子已经抵达了对面，判胜
    _userColor tempTargetColor;    //当前用户的目标色（当前玩家要抵达的棋槽的颜色）
    int userFinalPiece = 0;        //当前用户的抵达终点的棋子数
    switch (this->inWhichRoom->curUser->getUserColor())
    {
        case uyellow:
            tempTargetColor = ublue;
            break;
        case ublue:
            tempTargetColor = uyellow;
            break;
        case ured:
            tempTargetColor = ucyan;
            break;
        case ucyan:
            tempTargetColor = ured;
            break;
        case upurple:
            tempTargetColor = ugreen;
            break;
        case ugreen:
            tempTargetColor = upurple;
            break;
        default:
            break;
    }
    for (int i = 0; i <= 24; i++)
    {
        for (int j = 0; j <= 16; j++) // 遍历所有的棋槽，如果发现和当前目标颜色同色的棋槽
        {
            if (int(tempTargetColor) == int(this->inWhichRoom->coords[i][j].targetColor))   // 如果这个棋槽的目标色和玩家的目标色相吻合
            {
                if (int(this->inWhichRoom->coords[i][j].getColor()) == int(this->inWhichRoom->curUser->getUserColor()))   //如果这个棋槽上面的棋子的颜色和当前玩家的颜色相吻合
                {
                    userFinalPiece++;  // 成功到达终点的棋子数+1
                }
            }
        }
    }
    qDebug() << "userFinalPiece = " << userFinalPiece;
    if (userFinalPiece == 10)  //如果这名用户到达终点的棋子有10颗
    {
        this->inWhichRoom->curUser->isWin = true;

        qDebug() << this->inWhichRoom->curUser->userName << " has won";

        //将玩家加入胜利者列表中的空位
        for (int i = 1; i <= 6; i++)
        {
            if (this->inWhichRoom->winnerList[i].userName == "") // 如果玩家中某一项的名字为空
            {
                this->inWhichRoom->winnerList[i].userName = this->inWhichRoom->curUser->userName;
                break;
            }
        }

        //告知所有玩家这一信息
//        for (int i = 1; i <= this->inWhichRoom->userCnt; i++)
//        {
//            server_send_END_TURN(this->inWhichRoom->playerList[i].socket);
//        }
        server_send_END_TURN(this->inWhichRoom->curUser->socket);
        update();              //TODO unsovled：应该在界面上显示玩家已经获胜的信息
    }

    //结束终局判断之后，检查所有玩家的胜利或失败状态
    int overCnt = 0;
    for (int i = 1; i <= 6; i++)
    {
        if (this->inWhichRoom->users[i].isWin == true || this->inWhichRoom->users[i].isFail == true)
        {
            overCnt++;
        }
    }
    if (overCnt == this->inWhichRoom->userCnt - 1)   //如果当前仅剩最后一个人
    {
        qDebug() << "只剩最后一个人啦";

        //制作胜利者列表
        QString tempWinnerList = "";
        User *lastUser;
        for (int i = 1; i <= 6; i++)  // 找到最后一个人
        {
            if (this->inWhichRoom->users[i].isWin == false && this->inWhichRoom->users[i].isFail == false && this->inWhichRoom->users[i].userName != "")
            {
                lastUser = &this->inWhichRoom->users[i];   // 这里应该增加玩家的名字不为空的条件
                qDebug() << "the last user is " << lastUser->userName;
                break;
            }
        }
        for (int i = 1; i <= 6; i++)  // 找到空位
        {
            if (this->inWhichRoom->winnerList[i].userName == "")  // 如果当前位置为空，将这名玩家加入
            {
                this->inWhichRoom->winnerList[i].userName = lastUser->userName;
                break;
            }
        }

        qDebug() << "winnerList as follow:";

        for (int i = 1; i <= 6; i++)
        {
            qDebug() << this->inWhichRoom->winnerList[i].userName;
        }

        for (int i = 1; i <= 6; i++)
        {
            if (this->inWhichRoom->winnerList[i].userName != "") // 如果当前玩家的名字不为空
            {
                tempWinnerList.append(this->inWhichRoom->winnerList[i].userName);  //将这名用户的名字加到玩家列表字符串中
                tempWinnerList.append(" ");//用空格隔开
            }
        }
        tempWinnerList.chop(1); //删去末尾的空格
        for (int i = 1; i <= this->inWhichRoom->userCnt; i++)
        {
            server_send_END_GAME(this->inWhichRoom->playerList[i].socket, tempWinnerList); //向所有客户端发送这则消息
        }

        qDebug() << "Game over";

        this->inWhichRoom->clearRoom();

//        for (int i = 1; i <= MAX_ROOMCNT; i++)
//        {
//            if (mysystem->room[i].START_GAME == true)
//            {
//                mysystem->room[i].chooseThisRoom();
//            }
//        }

        return;  //游戏结束，直接退出
    }

    //结束终局判断之后，调用next，表示下一位玩家开始下棋
    this->inWhichRoom->next();

    // qDebug() << "server_receive_move_2";

    //轮次统计和记录每一次的棋盘状态
    this->inWhichRoom->recordState();

    // qDebug() << "server_receive_move_3";

    // 结束终局判断之后，告知所有用户下一个玩家应该开始下棋（此时已经调用next）
    for (int i = 1; i <= this->inWhichRoom->userCnt; i++)
    {
        server_send_START_TURN(this->inWhichRoom->playerList[i].socket, NetworkData(OPCODE::START_TURN_OP, this->inWhichRoom->curUser->zoom, QString::number(time(NULL))));
    }

    // qDebug() << "server_receive_move_4";

    //在服务端系统为下一轮的玩家调用倒计时
    this->inWhichRoom->countdown->setSecs(TIME);//将倒计时设置为三十秒
    this->inWhichRoom->countdown->setVisible(true);//呈现倒计时
    this->inWhichRoom->countdown->start();//倒计时开始
}

void Board::server_send_MOVE(QTcpSocket * socket, QString zoom, QString track)
{
    mysystem->server->send(socket, NetworkData(OPCODE::MOVE_OP, zoom, track));
}

void Board::server_send_END_TURN(QTcpSocket *socket)
{
    mysystem->server->send(socket, NetworkData(OPCODE::END_TURN_OP, "", ""));
}

void Board::server_send_END_GAME(QTcpSocket *socket, QString winnerList)
{
    mysystem->server->send(socket, NetworkData(OPCODE::END_GAME_OP, winnerList, ""));
}

void Board::server_send_ERROR(QTcpSocket * socket, QString detail)
{
    if (detail == "Soory, please enter your room number") // 房间名为空
    {
        mysystem->server->send(socket, NetworkData(OPCODE::ERROR_OP, "INVALID_JOIN", detail));
    }
    if (detail == "Sorry, You have the same name as the user in the room") // 名字相同
    {
        mysystem->server->send(socket, NetworkData(OPCODE::ERROR_OP, "INVALID_JOIN", detail));
    }
    if (detail == "Sorry, You are supposed to enter your name") // 名字为空
    {
        mysystem->server->send(socket, NetworkData(OPCODE::ERROR_OP, "INVALID_JOIN", detail));
    }
    if (detail == "Sorry, the room is full") // 房间已满
    {
        mysystem->server->send(socket,  NetworkData(OPCODE::ERROR_OP, "INVALID_JOIN", detail));
    }
    if (detail == "Sorry, there are no vacant rooms") // 所有房间都已经被占用，不能再新创建房间
    {
        mysystem->server->send(socket, NetworkData(OPCODE::ERROR_OP, "OTHER_OP", detail));
    }
    if (detail == "Sorry, the game in this room is begin") // 房间内游戏已经开始（不能再加入）
    {
        mysystem->server->send(socket, NetworkData(OPCODE::ERROR_OP, "ROOM_IS_RUNNING", detail));
    }
    if (detail == "Sorry, you are not in any of the rooms")  // 不在任何房间内（不能下棋）
    {
        mysystem->server->send(socket, NetworkData(OPCODE::ERROR_OP, "NOT_IN_ROOM", detail));
    }
    if (detail == "Sorry, your jump is invalid") // 非法跳跃
    {
        mysystem->server->send(socket, NetworkData(OPCODE::ERROR_OP, "INVALID_MOVE", detail));
    }
    if (detail == "Sorry, you are not in your turn") // 不在自己的回合内
    {
        mysystem->server->send(socket, NetworkData(OPCODE::ERROR_OP, "OUTTURN_MOVE", detail));
    }
}

//--------------------------------------------特殊错误处理函数--------------------------------------------------

//移动错误信息处理
void Board::movePieceBack()
{
    // qDebug() << "enter movePiece";

    // 将终点的颜色复制到起点
    this->inWhichRoom->coords[this->inWhichRoom->curUser->moveTrack[1][0].coord_x + 12][this->inWhichRoom->curUser->moveTrack[1][0].coord_y + 8].setColor(this->inWhichRoom->coords[this->inWhichRoom->curUser->moveTrack[this->inWhichRoom->curUser->jumpTime][1].coord_x + 12][this->inWhichRoom->curUser->moveTrack[this->inWhichRoom->curUser->jumpTime][1].coord_y + 8].getColor());

    // 将终点的颜色设置为空
    this->inWhichRoom->coords[this->inWhichRoom->curUser->moveTrack[this->inWhichRoom->curUser->jumpTime][1].coord_x + 12][this->inWhichRoom->curUser->moveTrack[this->inWhichRoom->curUser->jumpTime][1].coord_y + 8].setColor(null);

    // 将起点选作被选中的棋子
    this->inWhichRoom->choose(this->inWhichRoom->coords[this->inWhichRoom->curUser->moveTrack[1][0].coord_x + 12][this->inWhichRoom->curUser->moveTrack[1][0].coord_y + 8]);

    //搜索可行性
    this->inWhichRoom->search();
}

void Board::server_find_TIME_OUT()
{
    // 将这个用户加入胜利者列表的末端
    for (int i = 6; i >= 1; i--)
    {
        if (this->inWhichRoom->winnerList[i].userName == "")
        {
            this->inWhichRoom->winnerList[i].userName = this->inWhichRoom->curUser->userName;
            break;             //只添加一次
        }
    }

    // 通知所有棋子这一信息
    for (int i = 1; i <= this->inWhichRoom->userCnt; i++)
    {
        server_send_MOVE(this->inWhichRoom->playerList[i].socket, this->inWhichRoom->curUser->zoom, "-1");
    }

    // 设置服务端图形界面信息
    for (int i = 0; i <= 24; i++)
    {
        for (int j = 0; j <= 16; j++)
        {
            if (int(this->inWhichRoom->coords[i][j].getColor()) == int(this->inWhichRoom->curUser->getUserColor()))  // 找出所有颜色和当前用户颜色相同的棋子
            {
                this->inWhichRoom->coords[i][j].setColor(null);    // 将颜色和当前用户颜色相同的棋子清空
            }
        }
    }

    this->inWhichRoom->curUser->isFail = true;  // 将当前用户的是否负状态设置为是，并且在paintEvent中加入相关的显示流程
    update();                // 重绘

    // 如果倒数第二个玩家刚好是以这种方式结局
    int overCnt = 0;
    for (int i = 1; i <= 6; i++)
    {
        if (this->inWhichRoom->users[i].isFail == true || this->inWhichRoom->users[i].isWin == true)
        {
            overCnt++;
        }
    }
    if (overCnt == this->inWhichRoom->userCnt - 1)
    {
        //制作胜利者列表
        QString tempWinnerList = "";
        User *lastUser;
        for (int i = 1; i <= 6; i++)  // 找到最后一个人
        {
            if (this->inWhichRoom->users[i].isWin == false && this->inWhichRoom->users[i].isFail == false && this->inWhichRoom->users[i].userName != "")
            {
                lastUser = &this->inWhichRoom->users[i];
                break;
            }
        }
        for (int i = 1; i <= 6; i++)  // 找到空位
        {
            if (this->inWhichRoom->winnerList[i].userName == "")  // 如果当前位置为空，将这名玩家加入
            {
                this->inWhichRoom->winnerList[i].userName = lastUser->userName;
                break;
            }
        }

        for (int i = 1; i <= 6; i++)
        {
            if (this->inWhichRoom->winnerList[i].userName != "") // 如果当前玩家的名字不为空
            {
                tempWinnerList.append(this->inWhichRoom->winnerList[i].userName);  //将这名用户的名字加到胜利者列表字符串中
                tempWinnerList.append(" ");//用空格隔开
            }
        }
        tempWinnerList.chop(1); //删去末尾的空格

        qDebug() << "tempWinnerList is " << tempWinnerList;

        for (int i = 1; i <= this->inWhichRoom->userCnt; i++)
        {
            server_send_END_GAME(this->inWhichRoom->playerList[i].socket, tempWinnerList); //向所有客户端发送这则消息
        }
        qDebug() << "Game over";
        this->inWhichRoom->clearRoom();  // 清理房间
//        if (this->inWhichRoom == mysystem->curRoom) // 如果该房间就是当前显示的房间
//        {
//            for (int i = 1; i <= MAX_ROOMCNT; i++) // 找到从前往后数的第一间没有被清空的房间
//            {
//                if (mysystem->room[i].START_GAME == true)
//                {
//                    mysystem->room[i].chooseThisRoom();
//                }
//            }
//        }
        return;  //游戏结束,直接退出,不再调用next
    }

    this->inWhichRoom->next();                  // 如果场上还有用户,强制next,在服务端发生了curUser的变化，jumpTime的清零，nextImag图像的变化

    // 结束终局判断之后，告知所有用户下一个玩家应该开始下棋（此时已经调用next）
    for (int i = 1; i <= this->inWhichRoom->userCnt; i++)
    {
        server_send_START_TURN(this->inWhichRoom->playerList[i].socket, NetworkData(OPCODE::START_TURN_OP, this->inWhichRoom->curUser->zoom, QString::number(time(NULL))));
    }

    //在服务端系统照常开始倒计时
    this->inWhichRoom->countdown->setSecs(TIME);//将倒计时设置为三十秒
    this->inWhichRoom->countdown->setVisible(true);//呈现倒计时
    this->inWhichRoom->countdown->start();//倒计时开始
}

