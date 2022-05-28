#include "qwhtimimg.h"
#include "room.h"
#include <QDebug>

extern Coord NOTCHOSEN;



// 单人模式也是用myRoom
Room *myRoom;  // 客户端调用的房间，存储客户端的信息，棋盘的状态，里面的信息是用来支持客户端进行本地判断和绘图的

Room::Room()
{
    qDebug()<<"enter room";
    this->board->inWhichRoom = this;
    this->countdown->inWhichRoom = this;
    this->initCoords();
    this->board = new Board();
    this->countdown = new QWHTimimg(board);
    this->curChosenCoord = NOTCHOSEN;
    qDebug()<<"leave room";
}

// 数组坐标和系统坐标的转换函数
void Room::initCoords()
{
    for (int i = 0; i <= 24; i++)
    {
        for (int j = 0; j <= 16; j++)
        {
            coords[i][j].coord_x = i - 12;
            coords[i][j].coord_y = j - 8;
            coords[i][j].setHaveSlot(false);
        }
    }
    qDebug() << "Coords have been init";
}

//初始化用户(这个初始化操作不包括对于用户名的设置（这是在joinRoom以后进行的）)
void Room::initUsers()
{
    users[0].zoom = "O";
    users[1].zoom = "A";
    users[2].zoom = "B";
    users[3].zoom = "C";
    users[4].zoom = "D";
    users[5].zoom = "E";
    users[6].zoom = "F";

    for (int i = 0; i <= 6; i++)
    {
        users[i].id = i;            //设置初始化用户的id
        users[i].initMoveTrack();   //初始化用户的行棋轨迹
    }

    switch (userCnt)
    {
        case 2:
            users[1].setUserColor(uyellow);
            users[2].setUserColor(unull);
            users[3].setUserColor(unull);
            users[4].setUserColor(ublue);
            users[5].setUserColor(unull);
            users[6].setUserColor(unull);
            break;

        case 3:
            users[1].setUserColor(uyellow);
            users[2].setUserColor(unull);
            users[3].setUserColor(upurple);
            users[4].setUserColor(unull);
            users[5].setUserColor(ucyan);
            users[6].setUserColor(unull);
            break;

        case 6:
            users[1].setUserColor(uyellow);
            users[2].setUserColor(ured);
            users[3].setUserColor(upurple);
            users[4].setUserColor(ublue);
            users[5].setUserColor(ucyan);
            users[6].setUserColor(ugreen);
            break;
    }

    curUser = &users[1];
}

void Room::choose(Coord& chosenCoord)
{
    qDebug() << "enter choose!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    curChosenCoord = chosenCoord;
    coords[curChosenCoord.coord_x + 12][curChosenCoord.coord_y + 8].setChosenState(true);
}

void Room::search()
{
    // 如果是当前用户的第一次跳跃
    if (curUser->jumpTime == 0)
    {
        // qDebug() << "enter jumpTime = 0 jump";

        //相邻跳跃可行性
        //向六个方向相邻位置搜索可行位置，如果上面没有棋子并且有棋槽，就可以跳往
        //-1,+1
        if (curChosenCoord.coord_x >= -11 && curChosenCoord.coord_y <= 7 && (coords[curChosenCoord.coord_x + 12 - 1][curChosenCoord.coord_y + 8 + 1].getColor() == null)
                && (coords[curChosenCoord.coord_x + 12 - 1][curChosenCoord.coord_y + 8 + 1].getHaveSlot() == true))
        {
            coords[curChosenCoord.coord_x + 12 - 1][curChosenCoord.coord_y + 8 + 1].setFeasibility(true);
//            qDebug() << "feasibility: " << coords[curChosenCoord.coord_x + 12 - 1][curChosenCoord.coord_y + 8 + 1].coord_x
//                     << " " << coords[curChosenCoord.coord_x + 12 - 1][curChosenCoord.coord_y + 8 + 1].coord_y;
        }

        //-2 0
        if (curChosenCoord.coord_x >= -10 && (coords[curChosenCoord.coord_x + 12 - 2][curChosenCoord.coord_y + 8].getColor() == null)
                && (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y + 8].getHaveSlot() == true))
        {
            coords[curChosenCoord.coord_x + 12 - 2][curChosenCoord.coord_y + 8].setFeasibility(true);
//            qDebug() << "feasibility: " << coords[curChosenCoord.coord_x + 12 - 2][curChosenCoord.coord_y + 8].coord_x
//                     << " " << coords[curChosenCoord.coord_x + 12 - 2][curChosenCoord.coord_y + 8].coord_y;
        }

        //-1 -1
        if (curChosenCoord.coord_x >= -11 && curChosenCoord.coord_y >= -7 && (coords[curChosenCoord.coord_x + 12 - 1][curChosenCoord.coord_y + 8 - 1].getColor() == null)
                && (coords[curChosenCoord.coord_x + 12 - 1][curChosenCoord.coord_y + 8 - 1].getHaveSlot() == true))
        {
            coords[curChosenCoord.coord_x + 12 - 1][curChosenCoord.coord_y + 8 - 1].setFeasibility(true);
//            qDebug() << "feasibility: " << coords[curChosenCoord.coord_x + 12 - 1][curChosenCoord.coord_y + 8 - 1].coord_x
//                     << " " << coords[curChosenCoord.coord_x + 12 - 1][curChosenCoord.coord_y + 8 - 1].coord_y;
        }

        //1 1
        if (curChosenCoord.coord_x <= 11 && curChosenCoord.coord_y <= 7 && (coords[curChosenCoord.coord_x + 12 + 1][curChosenCoord.coord_y + 8 + 1].getColor() == null)
                && (coords[curChosenCoord.coord_x + 12 + 1][curChosenCoord.coord_y + 8 + 1].getHaveSlot() == true))
        {
            coords[curChosenCoord.coord_x + 12 + 1][curChosenCoord.coord_y + 8 + 1].setFeasibility(true);
//            qDebug() << "feasibility: " << coords[curChosenCoord.coord_x + 12 + 1][curChosenCoord.coord_y + 8 + 1].coord_x
//                     << " " << coords[curChosenCoord.coord_x + 12 + 1][curChosenCoord.coord_y + 8 + 1].coord_y;
        }

        //2 0
        if (curChosenCoord.coord_x <= 10 && (coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 8].getColor() == null)
                && (coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 8].getHaveSlot() == true))
        {
            coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 8].setFeasibility(true);
//            qDebug() << "feasibility: " << coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 8].coord_x
//                     << " " << coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 8].coord_y;
        }

        // 1 -1
        if (curChosenCoord.coord_x <= 11 && curChosenCoord.coord_y >= -7 && (coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y - 1 + 8].getColor() == null)
                && (coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y - 1 + 8].getHaveSlot() == true))
        {
            coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y - 1 + 8].setFeasibility(true);
//            qDebug() << "feasibility: " << coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y - 1 + 8].coord_x
//                     << " " << coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y - 1 + 8].coord_y;
        }

        //间隔跳跃可行性
        //-1 1
        if (curChosenCoord.coord_x >= -10 && curChosenCoord.coord_y <= 6 &&
                (coords[curChosenCoord.coord_x - 1 + 12][curChosenCoord.coord_y + 1 + 8].getColor() != null)
                && (coords[curChosenCoord.coord_x - 1 + 12][curChosenCoord.coord_y + 1 + 8].getHaveSlot() == true))
        {
            if ( (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y + 2 + 8].getColor() == null)
                    && (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y + 2 + 8].getHaveSlot() == true))
            {
                coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y + 2 + 8].setFeasibility(true);
            }
        }

        if (curChosenCoord.coord_x >= -8 && (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y + 8].getColor() != null)
                && (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y + 8].getHaveSlot() == true))
        {
            if ( (coords[curChosenCoord.coord_x - 4 + 12][curChosenCoord.coord_y + 8].getColor() == null)
                    && (coords[curChosenCoord.coord_x - 4 + 12][curChosenCoord.coord_y + 8].getHaveSlot() == true))
            {
                coords[curChosenCoord.coord_x - 4 + 12][curChosenCoord.coord_y + 8].setFeasibility(true);
            }
        }

        if (curChosenCoord.coord_x >= -10 && curChosenCoord.coord_y >= -6 &&
                (coords[curChosenCoord.coord_x - 1 + 12][curChosenCoord.coord_y - 1 + 8].getColor() != null)
                && (coords[curChosenCoord.coord_x - 1 + 12][curChosenCoord.coord_y - 1 + 8].getHaveSlot() == true))
        {
            if ( (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y - 2 + 8].getColor() == null)
                    && (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y - 2 + 8].getHaveSlot() == true))
            {
                coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y - 2 + 8].setFeasibility(true);
            }
        }

        if (curChosenCoord.coord_x <= 10 && curChosenCoord.coord_y <= 6 &&
                (coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y + 1 + 8].getColor() != null)
                && (coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y + 1 + 8].getHaveSlot() == true))
        {
            if ((coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 2 + 8].getColor() == null)
                    && (coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 2 + 8].getHaveSlot() == true))
            {
                coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 2 + 8].setFeasibility(true);
            }
        }


        if (curChosenCoord.coord_x <= 8 && (coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 8].getColor() != null)
                && (coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 8].getHaveSlot() == true))
        {
            if ( (coords[curChosenCoord.coord_x + 4 + 12][curChosenCoord.coord_y + 8].getColor() == null)
                    && (coords[curChosenCoord.coord_x + 4 + 12][curChosenCoord.coord_y + 8].getHaveSlot() == true))
            {
                coords[curChosenCoord.coord_x + 4 + 12][curChosenCoord.coord_y + 8].setFeasibility(true);
            }
        }


        if (curChosenCoord.coord_x <= 10 && curChosenCoord.coord_y >= -6 &&
                (coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y - 1 + 8].getColor() != null)
                && (coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y - 1 + 8].getHaveSlot() == true))
        {
            if ( (coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y - 2 + 8].getColor() == null)
                    && (coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y - 2 + 8].getHaveSlot() == true))
            {
                coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y - 2 + 8].setFeasibility(true);
            }
        }
    }

    //用户在此步搜索之前已经完成了一次跳跃
    if (curUser -> jumpTime != 0)
    {
        // qDebug() << "enter jumpTime != 0 senarch";

        //间隔跳跃可行性
        //-1 1
        if (curChosenCoord.coord_x >= -10 && curChosenCoord.coord_y <= 6 &&
                (coords[curChosenCoord.coord_x - 1 + 12][curChosenCoord.coord_y + 1 + 8].getColor() != null)
                && (coords[curChosenCoord.coord_x - 1 + 12][curChosenCoord.coord_y + 1 + 8].getHaveSlot() == true))
        {
            if ( (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y + 2 + 8].getColor() == null)
                    && (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y + 2 + 8].getHaveSlot() == true))
            {
                coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y + 2 + 8].setFeasibility(true);
            }
        }

        if (curChosenCoord.coord_x >= -8 && (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y + 8].getColor() != null)
                && (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y + 8].getHaveSlot() == true))
        {
            if ( (coords[curChosenCoord.coord_x - 4 + 12][curChosenCoord.coord_y + 8].getColor() == null)
                    && (coords[curChosenCoord.coord_x - 4 + 12][curChosenCoord.coord_y + 8].getHaveSlot() == true))
            {
                coords[curChosenCoord.coord_x - 4 + 12][curChosenCoord.coord_y + 8].setFeasibility(true);
            }
        }

        if (curChosenCoord.coord_x >= -10 && curChosenCoord.coord_y >= -6 &&
                (coords[curChosenCoord.coord_x - 1 + 12][curChosenCoord.coord_y - 1 + 8].getColor() != null)
                && (coords[curChosenCoord.coord_x - 1 + 12][curChosenCoord.coord_y - 1 + 8].getHaveSlot() == true))
        {
            if ( (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y - 2 + 8].getColor() == null)
                    && (coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y - 2 + 8].getHaveSlot() == true))
            {
                coords[curChosenCoord.coord_x - 2 + 12][curChosenCoord.coord_y - 2 + 8].setFeasibility(true);
            }
        }

        if (curChosenCoord.coord_x <= 10 && curChosenCoord.coord_y <= 6 &&
                (coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y + 1 + 8].getColor() != null)
                && (coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y + 1 + 8].getHaveSlot() == true))
        {
            if ((coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 2 + 8].getColor() == null)
                    && (coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 2 + 8].getHaveSlot() == true))
            {
                coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 2 + 8].setFeasibility(true);
            }
        }


        if (curChosenCoord.coord_x <= 8 && (coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 8].getColor() != null)
                && (coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y + 8].getHaveSlot() == true))
        {
            if ( (coords[curChosenCoord.coord_x + 4 + 12][curChosenCoord.coord_y + 8].getColor() == null)
                    && (coords[curChosenCoord.coord_x + 4 + 12][curChosenCoord.coord_y + 8].getHaveSlot() == true))
            {
                coords[curChosenCoord.coord_x + 4 + 12][curChosenCoord.coord_y + 8].setFeasibility(true);
            }
        }


        if (curChosenCoord.coord_x <= 10 && curChosenCoord.coord_y >= -6 &&
                (coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y - 1 + 8].getColor() != null)
                && (coords[curChosenCoord.coord_x + 1 + 12][curChosenCoord.coord_y - 1 + 8].getHaveSlot() == true))
        {
            if ( (coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y - 2 + 8].getColor() == null)
                    && (coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y - 2 + 8].getHaveSlot() == true))
            {
                coords[curChosenCoord.coord_x + 2 + 12][curChosenCoord.coord_y - 2 + 8].setFeasibility(true);
            }
        }
    }
}

void Room::cancelChoose()
{
    // qDebug() << "cancel!!!!!!!!!!!!!!!!";
    coords[curChosenCoord.coord_x + 12][curChosenCoord.coord_y + 8].setChosenState(false);
    curChosenCoord = NOTCHOSEN;
}

void Room::cancelFeasibility()
{
    for (int i = 0; i <= 24; i++)
    {
        for (int j = 0; j <= 16; j++)
        {
            coords[i][j].setFeasibility(false);
        }
    }
}
