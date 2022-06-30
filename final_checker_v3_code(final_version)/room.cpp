#include "room.h"
#include "ui_room.h"
#include "qwhtimimg.h"
#include <QDebug>
#include "system.h"
#include "chooseround.h"
#include "chooseroom.h"

extern Coord center;
extern Coord nullCoord;
extern Coord NOTCHOSEN;
extern System *mysystem;

// 单人模式也是用myRoom
Room *myRoom;  // 客户端调用的房间，存储客户端的信息，棋盘的状态，里面的信息是用来支持客户端进行本地判断和绘图的
// ??这个myRoom是不是没用了呀

extern User *me;

State::State()
{
    for (int i = 0 ; i <= 24; i++)
    {
        for (int j = 0; j <= 16; j++)
        {
            this->coordsState[i][j] = nullCoord;
            this->coordsState[i][j].setHaveSlot(false);
        }
    }
}

State::State(State& temp)
{
    for (int i = 0; i <= 24; i++)
    {
        for (int j = 0; j <= 16; j++)
        {
            this->coordsState[i][j] = temp.coordsState[i][j];
        }
    }
}

Round::Round()
{

}

Room::Room(QWidget *parent) : QWidget(parent), ui(new Ui::Room)
{
    ui->setupUi(this);

    // 设置棋盘控件的属性
    this->board = new Board();
    this->board->inWhichRoom = this;
    this->board->setParent(this);
    this->board->setGeometry(0, 0, this->width(), this->height());
    this->board->raise();
    this->board->setVisible(true);

    // 设置倒计时控件的属性
    this->countdown = new QWHTimimg(this->board, this->board);
    this->countdown->inWhichRoom = this;
    this->countdown->setVisible(false);

    // 初始化选择轮数的界面
    this->chooseRound = new ChooseRound();
    this->chooseRound->setParent(this);
    this->chooseRound->inWhichRoom = this;
    this->chooseRound->setGeometry(200, 50, 801, 491);

    // 初始化选择房间的界面
    this->chooseRoom = new ChooseRoom();
    this->chooseRoom->setParent(this);
    this->chooseRoom->inWhichRoom = this;
    this->chooseRoom->setGeometry(200, 50, 801, 491);

    this->initWidget();  // 初始化控件的可见性
    this->initCoords();  // 初始化坐标系统
    this->curChosenCoord = NOTCHOSEN;   // 初始化轮次系统

    // 设置鼠标图标
    QCursor cursor ;
    QPixmap pixmap(MOUSE) ;
    cursor = QCursor(pixmap, 0, 0);
    setCursor(cursor) ;
}

Room::~Room()
{
    delete ui;
    if (this->historyRoom != nullptr)
    {
        delete this->historyRoom;
    }
    if (this->originRoom != nullptr)
    {
        delete this->originRoom;
    }
}

void deserted2()
{
//void Room::hideCallRoom()
//{
//    this->ui->callRoom1->setVisible(false);
//    this->ui->callRoom2->setVisible(false);
//    this->ui->callRoom3->setVisible(false);
//    this->ui->callRoom4->setVisible(false);
//    this->ui->callRoom5->setVisible(false);
//    this->ui->callRoom6->setVisible(false);
//    this->ui->callRoom7->setVisible(false);
//    this->ui->callRoom8->setVisible(false);
//    this->ui->callRoom9->setVisible(false);
//    this->ui->callRoom10->setVisible(false);
//}

//void Room::showCallRoom()
//{
//    this->ui->callRoom1->setVisible(true);
//    this->ui->callRoom2->setVisible(true);
//    this->ui->callRoom3->setVisible(true);
//    this->ui->callRoom4->setVisible(true);
//    this->ui->callRoom5->setVisible(true);
//    this->ui->callRoom6->setVisible(true);
//    this->ui->callRoom7->setVisible(true);
//    this->ui->callRoom8->setVisible(true);
//    this->ui->callRoom9->setVisible(true);
//    this->ui->callRoom10->setVisible(true);
//}

//void Room::hideCallRoomImag()
//{
//    this->ui->callRoomImag->setVisible(false);
//}

//void Room::showCallRoomImag()
//{
//    this->ui->callRoomImag->setVisible(true);
//}
}

void Room::showNext()
{
    this->ui->next->setVisible(true);
}

void Room::showChangeRoom()
{
    this->ui->changeRoom->setVisible(true);
}

void Room::showHistory()
{
    this->ui->history->setVisible(true);
}

void Room::hideNext()
{
    this->ui->next->setVisible(false);
}

void Room::hideChangeRoom()
{
    this->ui->changeRoom->setVisible(false);
}

void Room::hideHistory()
{
    this->ui->history->setVisible(false);
}

void Room::hideCanOrNot()
{
    this->ui->you_can_you_up->setVisible(false);
    this->ui->no_can_no_BB->setVisible(false);
}

void Room::showCanOrNot()
{
    this->ui->you_can_you_up->setVisible(true);
    this->ui->no_can_no_BB->setVisible(true);
    this->ui->you_can_you_up->raise();
    this->ui->no_can_no_BB->raise();
}

Room* Room::buildHistoryRoom()
{
    // qDebug() << "enter buildHistory Room";
    Room *buildingRoom = new Room();

    // 设置buildingRoom的属性
    buildingRoom->mode = 3;        // 在没有点击you can you up之前
    buildingRoom->board->isServer = false;
    buildingRoom->roomNumber = "history"; // 设置房间号
    buildingRoom->userCnt = this->userCnt;       // 设置房间信息
    buildingRoom->historyRoom = nullptr;         // 历史房间设置为空（历史房间没有历史房间）
    buildingRoom->originRoom = this;             // 设置原始房间
    buildingRoom->IS_HISTORY_ROOM = true;        // 设置当前房间是历史房间

    // qDebug() << "the oringinRoom is " << buildingRoom->originRoom->roomNumber;

    this->historyRoom = buildingRoom;            // 设置当前房间的历史房间为这个
    buildingRoom->setVisible(false);             // 历史房间默认不显示???没必要啊
    // 完成historyRoom的一些初始化工作
    buildingRoom->initUsers();  //初始化用户
    buildingRoom->initCoords();

    return buildingRoom;
}

void Room::callHistoryRoom(int hisRound, _userColor hisUserColor)
{
    this->historyRoom->changeRoundState(hisRound, hisUserColor);
    this->historyRoom->update();
    this->historyRoom->show();
}

void Room::changeRoundState(int hisRound, _userColor hisUserColor)
{
    // 阻止不合理的查看行为 TODO
    // qDebug() << "enter changeRoundState";
    // 分配hisUserColor和state,调整hisRound,调整curUser

    // qDebug() << "the userCnt of historyRoom is " << this->userCnt;

//    if (hisUserColor == ublue)
//    {
//        qDebug() << "hisUserColor is blue";
//    }

    switch (this->userCnt)
    {
        case 2:
            if (hisUserColor == uyellow)
            {
                // qDebug() << "hisUserColor == uyellow---------------";
                hisRound--;  // 如果抽到的是黄色的，那么直接将轮次-1，移动到当前轮次的上一轮次
                for (int i = 0; i <= 24; i++)
                {
                    for (int j = 0; j <= 16; j++)
                    {
                        this->coords[i][j].setColor(this->round[hisRound].state[2].coordsState[i][j].getColor());  // 将选择的历史状态载入当前状态
                    }
                }
                this->curUser = &this->users[1];
            }
            else if (hisUserColor == ublue)
            {
                // qDebug() << " hisUserColor == ublue --------------- ";
                for (int i = 0; i <= 24; i++)
                {
                    for (int j = 0; j <= 16; j++)
                    {
                        this->coords[i][j].setColor(this->round[hisRound].state[1].coordsState[i][j].getColor());  // 将选择的历史状态载入当前状态
                    }
                }
                this->curUser = &this->users[4];
            }
            break;
        case 3:
            if (hisUserColor == uyellow)
            {
                hisRound--;  // 如果抽到的是黄色的，那么直接将轮次-1，移动到当前轮次的上一轮次
                for (int i = 0; i <= 24; i++)
                {
                    for (int j = 0; j <= 16; j++)
                    {
                        this->coords[i][j].setColor(this->round[hisRound].state[3].coordsState[i][j].getColor());  // 将选择的历史状态载入当前状态
                    }
                }
                this->curUser = &this->users[1];
            }
            else if (hisUserColor == upurple)
            {
                for (int i = 0; i <= 24; i++)
                {
                    for (int j = 0; j <= 16; j++)
                    {
                        this->coords[i][j].setColor(this->round[hisRound].state[1].coordsState[i][j].getColor());  // 将选择的历史状态载入当前状态
                    }
                }
                this->curUser = &this->users[3];
            }
            else if (hisUserColor == ucyan)
            {
                for (int i = 0; i <= 24; i++)
                {
                    for (int j = 0; j <= 16; j++)
                    {
                        this->coords[i][j].setColor(this->round[hisRound].state[2].coordsState[i][j].getColor());  // 将选择的历史状态载入当前状态
                    }
                }
                this->curUser = &this->users[5];
            }
            break;
        case 6:
            if (hisUserColor == uyellow)
            {
                hisRound--;  // 如果抽到的是黄色的，那么直接将轮次-1，移动到当前轮次的上一轮次
                for (int i = 0; i <= 24; i++)
                {
                    for (int j = 0; j <= 16; j++)
                    {
                        this->coords[i][j].setColor(this->round[hisRound].state[6].coordsState[i][j].getColor());  // 将选择的历史状态载入当前状态
                    }
                }
                this->curUser = &this->users[1];
            }
            else if (hisUserColor == ured)
            {
                for (int i = 0; i <= 24; i++)
                {
                    for (int j = 0; j <= 16; j++)
                    {
                        this->coords[i][j].setColor(this->round[hisRound].state[1].coordsState[i][j].getColor());  // 将选择的历史状态载入当前状态
                    }
                }
                this->curUser = &this->users[2];
            }
            else if (hisUserColor == upurple)
            {
                for (int i = 0; i <= 24; i++)
                {
                    for (int j = 0; j <= 16; j++)
                    {
                        this->coords[i][j].setColor(this->round[hisRound].state[2].coordsState[i][j].getColor());  // 将选择的历史状态载入当前状态
                    }
                }
                this->curUser = &this->users[3];
            }
            else if (hisUserColor == ublue)
            {
                for (int i = 0; i <= 24; i++)
                {
                    for (int j = 0; j <= 16; j++)
                    {
                        this->coords[i][j].setColor(this->round[hisRound].state[3].coordsState[i][j].getColor());  // 将选择的历史状态载入当前状态
                    }
                }
                this->curUser = &this->users[4];
            }
            else if (hisUserColor == ucyan)
            {
                for (int i = 0; i <= 24; i++)
                {
                    for (int j = 0; j <= 16; j++)
                    {
                        this->coords[i][j].setColor(this->round[hisRound].state[4].coordsState[i][j].getColor());  // 将选择的历史状态载入当前状态
                    }
                }
                this->curUser = &this->users[5];
            }
            else if (hisUserColor == ugreen)
            {
                for (int i = 0; i <= 24; i++)
                {
                    for (int j = 0; j <= 16; j++)
                    {
                        this->coords[i][j].setColor(this->round[hisRound].state[5].coordsState[i][j].getColor());  // 将选择的历史状态载入当前状态
                    }
                }
                this->curUser = &this->users[6];
            }
            break;
        default:
            break;
    }

    // 检查一下结果
//    for (int i = 0; i <= 24; i++)
//    {
//        for (int j = 0; j <= 16; j++)
//        {
//            if (this->coords[i][j].getColor() != null)
//            {
//                qDebug() << "the color of " << "(" << i << "," << j << ") is " << int(coords[i][j].getColor());
//            }
//            else
//            {
//                qDebug() << "the color of " << "(" << i << "," << j << ") is null";
//            }
//        }
//    }
}

void Room::chooseThisRoom()
{
    mysystem->curRoom->setVisible(false);
    mysystem->curRoom = this;
    mysystem->curRoom->setVisible(true);
    mysystem->curRoom->ui->changeRoom->setVisible(true);
    mysystem->curRoom->ui->history->setVisible(true);
    // qDebug() << "Change to Room " << mysystem->curRoom->roomNumber;
}

void Room::clickHistory()
{
    this->chooseRound->show();
}

void Room::you_can_you_up()
{
    this->ui->you_can_you_up->setVisible(false);
    this->mode = 1;
}

void Room::initWidget()
{
    // 将所有部件设置为不可见
    this->ui->next->setVisible(false);
    this->ui->changeRoom->setVisible(false);
    this->ui->history->setVisible(false);
    this->hideCanOrNot();
}

/**
 * @brief Room::initCoords 初始化坐标，设置坐标的横纵向量，设置坐标是否有棋槽
 */
void Room::initCoords()
{
    for (int i = 0; i <= 24; i++)
    {
        for (int j = 0; j <= 16; j++)
        {
            coords[i][j].coord_x = i - 12;
            coords[i][j].coord_y = j - 8;
            coords[i][j].setHaveSlot(false);
            coords[i][j].setColor(null);
        }
    }
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

void Room::initGame()
{
    // 设置所有棋子的颜色和目标颜色
    for (int i = 0; i <= 3; i++)
    {
        for (int j = 0; j <= 3 - i; j++)
        {
            this->coords[-3 + i + 2 * j + 12][5 + i + 8].targetColor = yellow;
            this->coords[-3  + i + 2 * j + 12][-5 - i + 8].targetColor = blue;
            this->coords[6 + 2 * i + j + 12][4 - j + 8].targetColor = red;
            this->coords[-6 - 2 * i - j + 12][-4 + j + 8].targetColor = cyan;
            this->coords[6 + 2 * i + j + 12][-4 + j + 8].targetColor = purple;
            this->coords[-6 - 2 * i - j + 12][4 - j + 8].targetColor = green;

            this->coords[-3 + i + 2 * j + 12][5 + i + 8].setColor(yellow);

            if ((this->userCnt == 6) || (this->userCnt == 2))
            {
                this->coords[-3  + i + 2 * j + 12][-5 - i + 8].setColor(blue);
                // this->coords[-3  + i + 2 * j + 12][-5 - i + 8].targetColor = blue;
            }

            if (this->userCnt == 6)
            {
                this->coords[6 + 2 * i + j + 12][4 - j + 8].setColor(red);
                // this->coords[6 + 2 * i + j + 12][4 - j + 8].targetColor = red;
            }

            if ((this->userCnt == 3) || (this->userCnt == 6))
            {
                this->coords[-6 - 2 * i - j + 12][-4 + j + 8].setColor(cyan);
                // this->coords[-6 - 2 * i - j + 12][-4 + j + 8].targetColor = cyan;
            }

            if ((this->userCnt == 3) || (this->userCnt == 6))
            {
                this->coords[6 + 2 * i + j + 12][-4 + j + 8].setColor(purple);
                // this->coords[6 + 2 * i + j + 12][-4 + j + 8].targetColor = purple;
            }

            if (this->userCnt == 6)
            {
                this->coords[-6 - 2 * i - j + 12][4 - j + 8].setColor(green);
                // this->coords[-6 - 2 * i - j + 12][4 - j + 8].targetColor = green;
            }
        }
    }

    // 设置当前位置是否有棋槽
    for (int i = 0; i <= 4; i++)
    {
        for (int j = -8 - i; j <= 8 + i; j += 2)
        {

            this->coords[j + 12][i + 8].setHaveSlot(true);
            this->coords[j + 12][-i + 8].setHaveSlot(true);
        }

        for (int j = -i; j <= i; j += 2)
        {
            this->coords[j + 12][8 - i + 8].setHaveSlot(true);
            this->coords[j + 12][-8 + i + 8].setHaveSlot(true);
        }
    }
}

/**
 * @brief Room::initRound 请在调用initCoords之后才调用该函数,初始化第0轮的内容
 */
void Room::initRound()
{
    for (int i = 1; i <= 6; i++)
    {
        for (int j = 1; j <= 24; j++)
        {
            for (int k = 1; k <= 16; k++)
            {
                this->round[0].state[i].coordsState[j][k] = coords[j][k];
            }
        }
    }
}

void Room::choose(Coord & chosenCoord)
{
    // qDebug() << "enter choose!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
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
        if (curChosenCoord.coord_x >= -10 && curChosenCoord.coord_y <= 6
                && (coords[curChosenCoord.coord_x - 1 + 12][curChosenCoord.coord_y + 1 + 8].getColor() != null)
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

/**
 * @brief Room::recordState 该函数直接将当前棋盘的状态记录到历史状态中
 */
void Room::recordState()
{
    if (this->historyRoom->round[this->historyRoom->curRoundCnt].curStateCnt == userCnt)  // 如果当前轮中储存的状态已经满了
    {
        qDebug() << "record 1";
        this->historyRoom->round[this->historyRoom->curRoundCnt].state[this->historyRoom->round[this->historyRoom->curRoundCnt].curStateCnt].beforeWho = this->curUser->getUserColor();
        for (int i = 0; i <= 24; i++)
        {
            for (int j = 0; j <= 16; j++)
            {
                // 下面这行代码用来将当前状态记录到历史状态的对象数组中
                this->historyRoom->round[this->historyRoom->curRoundCnt].state[this->historyRoom->round[this->historyRoom->curRoundCnt].curStateCnt].coordsState[i][j] = this->coords[i][j];
            }
        }
        this->historyRoom->curRoundCnt++;   // 本轮已经满了，重新开一轮
        this->historyRoom->round[this->historyRoom->curRoundCnt].roundNum = this->historyRoom->curRoundCnt;  // 将当前的轮数设置为新房间的轮号
    }
    else
    {
        qDebug() << "record 2";
        // ywt(2022/6/1): 您在检查作业时请不要试图看懂下面的代码，以节约宝贵的时间
        this->historyRoom->round[this->historyRoom->curRoundCnt].state[this->historyRoom->round[this->historyRoom->curRoundCnt].curStateCnt].beforeWho = this->curUser->getUserColor();
        for (int i = 0; i <= 24; ++i)
        {
            for (int j = 0; j <= 16; ++j)
                // 下面这行代码用来将当前状态记录到历史状态的对象数组中
            {
                this->historyRoom->round[this->historyRoom->curRoundCnt].state[this->historyRoom->round[this->historyRoom->curRoundCnt].curStateCnt].coordsState[i][j] = this->coords[i][j];
            }
        }
        this->historyRoom->round[this->historyRoom->curRoundCnt].curStateCnt++;   // 那一轮中的状态数增加
    }
}

//next函数用来转移操作权和当前操作用户
void Room::next()
{
    curUser->jumpTime = 0;

    qDebug() << "Enter next";

    // 要将当前用户的轨迹置为全部是nullCoord
    curUser->initMoveTrack();

    if (curUser->id == 6)
    {
        curUser = &users[1];    // 防越界处理（移到第一个成员）
    }
    else
    {
        curUser++;                              // curUser指向下一个User
    }

    // 如果当前指向的颜色为unull（空）,则再调用一次该函数(系统自动调用next)或者这名用户已经被判胜或判负
    if (curUser->getUserColor() == unull || curUser->isFail == true || curUser->isWin == true)
    {
        this->next();
        // qDebug() << "Because null next";
    }

    // 等到所有的next执行完毕后执行update
    this->board->update();

    // qDebug() << "the current user is" << curUser->userName;
}

// -----------------------------------------------------绘图函数相关

void Room::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

    // 如果没有打开则全部默认关闭
    this->hideCanOrNot();
    // this->hideChangeRoom();
    // this->hideHistory();
    this->hideNext();

    this->ui->round->setText(this->ui->round->text() + QString::number(this->curRoundCnt));
    this->ui->round->show();

    if (this->board->isServer == true)
    {
        this->ui->server->show();
        this->ui->client->close();
    }
    else
    {
        this->ui->server->show();
    }

    if (this->IS_HISTORY_ROOM == true)
    {
        // qDebug() << "enter historyRoom::paintEvent";
        // 设置电视图片背景
        this->setTVBackground();
        // 显示两个选择的按钮
        this->showCanOrNot();
        // 绘制棋盘
        this->board->update();
        this->board->show();
        return;
    }
    // 如果不是历史房间并且是，客户端模式或者单人模式，直接调用board的update然后return
    else if (this->mode == 1 || (this->mode == 3 && this->board->isServer == false))
    {
        if (this->USER_HAS_BEEN_INIT && this->curUser->jumpTime != 0)   // 客户端模式或单人模式可以点击 next
        {
            // this->inWhichRoom->nextImag(painter);
            this->showNext();
        }
        this->board->update();
        this->board->show();
        return;
    }
    // 如果不是历史房间并且是服务端模式，在调用board的update函数的基础上实现其它功能
    else if (this->mode == 3 && this->board->isServer == true)
    {
        this->showChangeRoom();
        this->showHistory();
        this->board->update();
        this->board->show();
        return;
    }
}

// 更新下一步按钮的画面
void Room::nextImag(QPainter & painter)
{
    painter.setPen(Qt::black);

    if (this->curUser->jumpTime == 0)
    {
        return;
    }

    // qDebug() << "2the User color is" << curUser->getUserColor();

    if (this->curUser->getUserColor() == uyellow)
    {
        // qDebug() << "Enter nextImag,the User color is yellow";
        painter.setBrush(QBrush(QPixmap(YELLOW)));
    }
    else if (this->curUser->getUserColor() == ured)
    {
        // qDebug() << "Enter nextImag,the User color is red";
        painter.setBrush(QBrush(QPixmap(RED)));
    }
    else if (this->curUser->getUserColor() == upurple)
    {
        // qDebug() << "Enter nextImag,the User color is purple";
        painter.setBrush(QBrush(QPixmap(PURPLE)));
    }
    else if (this->curUser->getUserColor() == ublue)
    {
        // qDebug() << "Enter nextImag,the User color is blue";
        painter.setBrush(QBrush(QPixmap(BLUE)));
    }
    else if (this->curUser->getUserColor() == ucyan)
    {
        // qDebug() << "Enter nextImag,the User color is cyan";
        painter.setBrush(QBrush(QPixmap(CYAN)));
    }
    else if (this->curUser->getUserColor() == ugreen)
    {
        // qDebug() << "Enter nextImag,the User color is green";
        painter.setBrush(QBrush(QPixmap(GREEN)));
    }
    else if (this->curUser->getUserColor() == unull)
    {
        // qDebug() << "Enter nextImag,the User color is white";
        painter.setBrush(QBrush(QPixmap(DESK)));    // 理论上这个白色是看不见的，因为next函数在遇到unull的情况就会立即跳过
    }

    QPoint points1[4] =
    {
        Coord(16, 6).coord2QPoint(), Coord(16, 4).coord2QPoint(),
        Coord(28, 4).coord2QPoint(), Coord(28, 6).coord2QPoint()
    };

    painter.drawPolygon(points1, 4);

}

void Room::setTVBackground()
{
    // 在参数中构造并引入desk图片
    this->ui->desk->setStyleSheet("QFrame{background-image: url(I:/coding/C++project/final_checker_v1/pictureSource/tv.png)}");
}

// ----------------------------------------------------鼠标点击函数相关

void Room::mouseReleaseEvent(QMouseEvent *event)
{
    // 获取当前点击的位置
    //  qDebug() << "mode is " << this->mode;
    // qDebug() << "IS_HISTORY_ROOM is " << this->IS_HISTORY_ROOM;
    // qDebug() << "START_GAME is " << this->START_GAME;

    QPoint curPoint = event->pos();

    // qDebug() << "the room click is " << this->roomNumber;

    if (this->mode == 3 && !this->START_GAME && this->board->isServer == false && this->IS_HISTORY_ROOM == false) //如果是客户端模式，并且还没有初始化一局游戏，并且是不是历史模式，直接return（避免点击到还没初始化的坐标）
    {
        return;
    }

    if ((this->mode == 3 && this->board->isServer == true) || (this->mode == 3 && this->IS_HISTORY_ROOM == true))
    {
        // 服务端设置的鼠标功能只有读取是否点击了历史
        if (this->IS_HISTORY_ROOM == false)  // 如果当前不是历史房间
        {
            // qDebug() << "enter  == false";

            if ((curPoint.x() >= 740)
                    && (curPoint.x() <= 740 + 161)
                    && (curPoint.y() >= 320)
                    && (curPoint.y() <= 320 + 71))
            {
                this->chooseRoom->show();
            }
            if ((curPoint.x() >= 740)
                    && (curPoint.x() <= 740 + 161)
                    && (curPoint.y() >= 450)
                    && (curPoint.y() <= 450 + 71))
            {
                this->chooseRound->show();
            }
        }
        if (this->IS_HISTORY_ROOM == true)   // 如果当前是历史房间
        {
            // qDebug() << "enter this->IS_HISTORY_ROOM == true";
            // qDebug() << "click (" << curPoint.x() << "," << curPoint.y() << ")";
            if ((curPoint.x() >= 1070)
                    && (curPoint.x() <= 1070 + 81)
                    && (curPoint.y() >= 180)
                    && (curPoint.y() <= 180 + 61))
            {
                // qDebug() << "click you_can_you_up";
                this->you_can_you_up();
            }
            if ((curPoint.x() >= 1078)
                    && (curPoint.x() <= 1078 + 71)
                    && (curPoint.y() >= 280)
                    && (curPoint.y() <= 280 + 51))
            {
                // qDebug() << "no can no BB";
                this->close();
            }
        }
    }

    if (this->mode == 1)   // 单人模式下允许所有角色的鼠标功能
    {
        {
            // 将当前的点击位置对应到具体的坐标(枚举法或二分查找，后续请进行优化)
            // Coord curCoord = getCoord(curPoint);
            // {
            // do语句的作用在于实现可取消行棋的操作
            // do
            // {
            // 重走一次就设置为false,如果又取消，在updateChosenState里面会进行重新设值
            // RECHOOSE = false;
            // if语句获取当前点击的位置上棋子的颜色并进行校对
            // 规定只能使用自己颜色的棋子
            // if (curCoord.getColor() == curUser.getUserColor())
            // {
            // 如果校对成功被选中棋子作出响应:
            //            // 响应一：修改自身的信息
            //            connect(&curCoord,
            //                    &Coord::updateChosenState,
            //                    this,
            //                    &Board::updateChosenState);
            //            // 响应二：使其它棋子处于不可选状态
            //            // 响应三：绘制选中效果（利用qpainter的重绘效果）
            //            connect(&curCoord,
            //                    &Coord::updateChosenState,
            //                    this,
            //                    &Board::updateChosenStateImag);
            //            // 将自身作为参数发送信号
            //            emit curCoord.updateChosenState();
            // 事实上，feasibility已经为我们此步打下了很好的铺垫
            // this->updateChosenState(curCoord);
            // 进行可行性分析，并进行可移动位置的标注（调用painter）
            // 初始设置跳动次数为0
            // this->updateFeasibility(curCoord, 0);
            // 再次获取选中的区域
            // 判断该区域是不是被标注过的区域
            // 清除标注，绘制棋子的移动，循环可行性分析的步骤
            // }
            // }while (RECHOOSE);
        }

        if (this->IS_HISTORY_ROOM == true)
        {
            if ((curPoint.x() >= 1078)
                    && (curPoint.x() <= 1078 + 71)
                    && (curPoint.y() >= 280)
                    && (curPoint.y() <= 280 + 51))
            {
                // qDebug() << "no can no BB";
                this->close();
            }
        }

        Coord curCoord = getCoord(curPoint);
        // 以下是用户自己点击的next
        // 是否点击next一定在是否点击棋槽之前
        // 如果当前是用户的第一次跳跃,并且试图点击 next，直接忽略其想要点击的操作，进行下一个if判断
        if (this->curUser->jumpTime != 0)
        {
            // qDebug() << "curUser'jumpTime is" << this->curUser->jumpTime;

            // 判断当前点是否在next按键的指定范围内，如果没有，退出这个if判断（不能直接return，因为用户有可能是在点棋槽）
            if ((curPoint.x() >= 740)
                    && (curPoint.x() <= 740 + 161)
                    && (curPoint.y() >= 190)
                    && (curPoint.y() <= 190 + 71))
                // 如果点在区域内,注意:这里有一个非常易错的地方，在我们的坐标系内，QPoint的纵坐标越大，Coord的纵坐标越小
            {
                //以下是点击next的操作

                // qDebug() << "user click next legally";

                // 撤销所有的可行性设置
                this->cancelFeasibility();

                // 撤销用户已经选择的棋子
                this->cancelChoose();

                // 要将这位用户的跳动次数置为0
                this->curUser->jumpTime = 0;

                this->next();
                // next函数里面已经包含了updata

                return;
            }
        }

        if (curCoord.coord_x == -100) // 如果返回的是空坐标，说明没点到棋子或棋槽，直接return
        {
            // qDebug() << "nullPlace";
            return;
        }

        // 点到了我们定义的coords
        // 如果当前界面没有选中的棋子
        if (this->curChosenCoord.coord_x == -100)
        {
            // qDebug() << "have not Chosen piece-----------------------------------------------------";

            // qDebug() << "clicked piece is " << curCoord.getColor() << "my color is " << curUser->getUserColor();

            //(你要先确保你点到的是棋子)
            if (curCoord.getColor() != null)
            {
                // 如果点到的是别人的棋子，说明点错了，直接return
                if (int(curCoord.getColor()) != int(this->curUser->getUserColor()))
                {
                    // qDebug() << "not my piece";
                    return;
                }
                // 如果点到的是自己的棋子，将其设为选中，并展示可行性
                else if (int(curCoord.getColor()) == int(this->curUser->getUserColor()))
                {
                    // qDebug() << "choose my piece--------------------------------------------------------------";

                    this->choose(curCoord);
                    this->search();  // search 依赖于curChosenUser指针，不用传参
                    this->board->update();        // 更新
                    return;
                }
            }
            else
            {
                return ;
            }
        }

        // 如果当前界面已经有选中的棋子了
        else
        {
            // qDebug() << "have piece chosen-----------------------------------------------------------";

            // qDebug() << "curCoord is " << curCoord.coord_x << " " << curCoord.coord_y;

            //如果点到的是自己的刚刚选中的棋子，执行取消操作,如果用户已经跳过棋了，则禁止这种行为
            if ((curCoord.coord_x == this->curChosenCoord.coord_x)
                    && (curCoord.coord_y == this->curChosenCoord.coord_y)
                    && this->curUser->jumpTime == 0)
            {
                // qDebug() << "cancle the piece-------------------------------------------------------";
                // 注意这三行代码的顺序先后
                // 清空原来棋子的选中状态，指针指向空地址，调用绘图函数更新图象

                this->cancelFeasibility();
                this->cancelChoose();
                this->board->update();
                return;
            }

            // 如果点到的不是自己刚刚选中的棋子

            //如果点到的是自己的其它棋子,并且这个用户当前还没有跳过棋（如果这个用户已经操作过了，则禁止这种行为）
            if (int(curCoord.getColor()) == int(this->curUser->getUserColor()) && this->curUser->jumpTime == 0)
            {
                this->cancelFeasibility();
                this->cancelChoose();
                this->choose(curCoord);
                this->search();
                this->board->update();
                return;
            }

            // 如果点到的是不能到达的位置，直接return
            if (curCoord.getFeasibility() == false)
            {
                return;
            }

            // 如果点到的是能到达的位置,绘制新的棋子布局图像,然后返回
            if (curCoord.getFeasibility() == true)
            {
                // qDebug() << "move piece";

                //这个位置虽然能够到达，但是这个位置在自己的历史轨迹上，我们应该阻止这种行为
                for (int i = 0; i <= 20; i++)
                {
                    //如果当前点击的棋子已经成为过某次跳跃的终点或者自己第一次跳跃的起点（这里不会和取消操作冲突，因为对取消操作的判断已经在前面进行过了），直接return
                    if (curCoord == this->curUser->moveTrack[i][1] || curCoord == this->curUser->moveTrack[1][0])
                    {
                        return;
                    }
                }

                /*TODO(ywt:2022\4\29)
                 * 测试一下新增加的这个阻止返回的功能
                 */

                this->board->move(this->curChosenCoord, curCoord);  //移动（我们允许在客户端先显示移动以后的画面，如果服务端检测到异常再进行强制遣返）

                //如果进行的是间隔跳跃
                if (abs(this->curChosenCoord.coord_x - curCoord.coord_x) >= 4
                        || abs(this->curChosenCoord.coord_y - curCoord.coord_y) >= 2)
                {
                    this->curUser->jumpTime++;  //跳跃次数+1

                    //记录轨迹
                    this->curUser->moveTrack[this->curUser->jumpTime][0] = this->curChosenCoord;
                    this->curUser->moveTrack[this->curUser->jumpTime][1] = curCoord;

                    //在用户点击next时才将jumpTime置为0（这里不置零）

                    this->cancelChoose();  //撤销已经选择的棋子
                    this->cancelFeasibility();  //撤销所有的可行性标注
                    this->choose(curCoord);      //选择跳到的位置作为当前选择的位置
                    this->search();              //搜索可行性
                    this->board->update();
                    return;
                }


                //如果进行的是相邻跳跃
                else
                {
                    this->curUser->jumpTime++;  //跳跃次数++
                    this->curUser->moveTrack[this->curUser->jumpTime][0] = this->curChosenCoord; //记录轨迹
                    this->curUser->moveTrack[this->curUser->jumpTime][1] = curCoord;
                    this->cancelChoose();  //撤销已经选择的棋子
                    this->cancelFeasibility();  //撤销所有的可行性标注
                    this->next();
                    this->board->update();
                    return;
                }
            }
        }
    }

    // 如果当前模式为3并且是客户端
    // 事实上并不需要在客户端将那些各种限制条件都删除，但是服务端上要有必要的判断功能
    // 事实上feasibility，chosenState，moveTrack, jumpTime等等，都是在服务端的辅助功能，并不需要我们进行信息传递，也不需要我们进行服务端的检测
    // 所以事实上我们要检测的内容只有：用户是不是点了next，用户的移动轨迹是什么

    if (this->mode == 3 && this->board->isServer == false)
    {
        // qDebug() << "enter this->mode == 3 && this->board->isServer == false";
        // debug语句：鼠标点击次数
        // realeseTime++;
        // qDebug() << "you click " << realeseTime << " times";

        // 获取鼠标当前点击的位置
        QPoint curPoint = event->pos();

        // 将鼠标当前点击的位置转换成鼠标当前点击的点
        Coord curCoord = getCoord(curPoint);

        // 以下是用户自己点击的next
        // 如果当前是用户的第一次跳跃,并且试图点击 next，直接忽略其想要点击的操作，进行下一个if判断

        //qDebug() << "My jumpTime is" << me->jumpTime;

        // TODO：服务端在这边应该能够识别用户是第几次跳跃，并对第一次跳跃就试图点击next（MOVE中必然传递空字符串）的行为进行报错

        if (this->curUser != me)  // 客户端应该增加的新功能:如果curUser和me指针指向的不是users数组中的同一个玩家对象，那么直接return，无视用户的所有操作
        {
            // qDebug() << "curUser is " << curUser->userName << " and me is " << me->userName;
            return;
        }

        if (me->jumpTime != 0)    //如果当前用户不是第一次跳跃
        {
            // 判断当前点是否在next按键的指定范围内，如果没有，退出这个if判断（不能直接return，因为用户有可能是在点棋槽）
            if ((curPoint.x() >= 740)
                    && (curPoint.x() <= 740 + 161)
                    && (curPoint.y() >= 190)
                    && (curPoint.y() <= 190 + 71))  // 如果点在区域内,注意:这里有一个非常易错的地方，在我们的坐标系内，QPoint的纵坐标越大，Coord的纵坐标越小
            {
                // qDebug() << "user click next";

                // 以下是网络模式下点击next后的操作
                // next被封装到了Board::client_receive_MOVE内
                // 当收到来自客户端的回复时，才调用next功能

                this->cancelFeasibility();   // 撤销所有的可行性设置

                this->cancelChoose();   // 撤销用户所有的选择

                /*todo：
                 *当收到来自客户端的错误信息时
                 *将棋子退回本回合的起始位置，并且前面已经将jumpTime置零，让玩家重新开始操作
                 */

                //重置轨迹的操作不能写在这里，要等收到客户端信息确认可以重置才进行重置

                this->board->client_send_MOVE();     //发送信息 //next();

                me->jumpTime = 0;  // 要将自己的跳动次数置为0

                this->board->update();               //这里允许现在客户端先显示移动的图像，如果出错再进行撤回

                return;
            }
        }

        if (curCoord.coord_x == -100) // 如果返回的是空坐标，说明没点到棋子或棋槽，直接return，就算是在联网状态下也无需传递信号
        {
            // qDebug() << "nullPlace";
            return;
        }

        if (this->curChosenCoord.coord_x == -100)  //如果当前界面没有被选中的棋子
        {
            // qDebug() << "have not Chosen piece-----------------------------------------------------";

            // qDebug() << "clicked piece is " << curCoord.getColor() << "my color is " << me->getUserColor();

            if (curCoord.getColor() != null)  //(你要先确保你点到的是棋子而不是棋槽)
            {
                if (int(curCoord.getColor()) != int(me->getUserColor()))   // 如果点到的是别人的棋子，说明点错了，直接return
                {
                    //  qDebug() << "not my piece";
                    return;
                }

                else if (int(curCoord.getColor()) == int(me->getUserColor()))  // 如果点到的是自己的棋子，将其设为选中，并展示可行性，选中棋子这一步不需要服务端的介入，也不需要接收服务端的反馈
                {
                    // qDebug() << "choose my piece--------------------------------------------------------------";

                    this->choose(curCoord);
                    this->search();   // search 依赖于curChosenUser指针，不用传参
                    this->board->update();     // 更新
                    return;
                }
            }
            else
            {
                return ;         //点到空棋槽的话直接return
            }
        }
        else       // 如果当前界面已经有选中的棋子了
        {
            // qDebug() << "have piece chosen-----------------------------------------------------------";

            // qDebug() << "curCoord is " << curCoord.coord_x << " " << curCoord.coord_y;

            //如果点到的是自己的刚刚选中的棋子，执行取消操作,如果用户已经操作过了，则禁止这种行为，取消操作是纯客户端的行为
            if ((curCoord.coord_x == this->curChosenCoord.coord_x)
                    && (curCoord.coord_y == this->curChosenCoord.coord_y)
                    && me->jumpTime == 0)
            {
                // qDebug() << "cancle the piece-------------------------------------------------------";
                // 注意这三行代码的顺序先后
                // 清空原来棋子的选中状态，指针指向空地址，调用绘图函数更新图象

                this->cancelFeasibility();
                this->cancelChoose();
                this->board->update();
                return;
            }

            // 以下表示如果点到的不是自己刚刚选中的棋子

            if (int(curCoord.getColor()) == int(me->getUserColor()) && me->jumpTime == 0)   //如果点到的是自己的其它棋子,并且这个用户当前还没有跳过棋，就执行换棋（如果这个用户已经操作过了，则禁止这种行为），中途换棋也是纯客户端的行为
            {
                this->cancelFeasibility();
                this->cancelChoose();
                this->choose(curCoord);
                this->search();
                this->board->update();
                return;
            }

            if (curCoord.getFeasibility() == false)  // 如果点到的是不能到达的位置，直接return
            {
                return;
            }

            if (curCoord.getFeasibility() == true) // 如果点到的是能到达的位置,绘制新的棋子布局图像,记录到轨迹中
            {
                // qDebug() << "move piece";

                for (int i = 0; i <= 20; i++)   //这个位置虽然能够到达，但是这个位置在自己的历史轨迹上，我们应该阻止这种行为，判定历史轨迹也是纯客户端的行为
                {
                    //如果当前点击的棋子已经成为过某次跳跃的终点或者自己第一次跳跃的起点，直接return
                    //（这里不会和取消操作冲突，因为对取消操作的判断已经在前面进行过了，不会和点击棋盘外的其它位置冲突，因为这个已经判断过了）
                    if (curCoord == me->moveTrack[i][1] || curCoord == me->moveTrack[1][0])
                    {
                        return;
                    }
                }

                this->board->move(this->curChosenCoord, curCoord);  //允许自己的客户端先进行移动

                if (abs(this->curChosenCoord.coord_x - curCoord.coord_x) >= 4
                        || abs(this->curChosenCoord.coord_y - curCoord.coord_y) >= 2)  //如果进行的是间隔跳跃
                {
                    me->jumpTime++;   //当点击next时会把jumpTime置为0

                    //记录轨迹（网络模式下，应该将move记录到自己的moveTrack里面）
                    me->moveTrack[me->jumpTime][0] = this->curChosenCoord;
                    me->moveTrack[me->jumpTime][1] = curCoord;
                    //moveTrack应该在得到服务端反馈以后才清空，因为如果接收到的是错误信息，我们还应该通过moveTrack进行遣返

                    this->cancelChoose();  //撤销已经选择的棋子
                    this->cancelFeasibility();  //撤销所有的可行性标注
                    this->choose(curCoord);      //选择跳到的位置作为当前选择的位置
                    this->search();              //搜索可行性
                    this->board->update();              //重绘
                    return;
                }

                // 如果进行的是相邻跳跃
                else
                {
                    me->jumpTime++;
                    me->moveTrack[me->jumpTime][0] = this->curChosenCoord;
                    me->moveTrack[me->jumpTime][1] = curCoord;

                    //me->jumpTime = 0;  在next中已经实现
                    this->cancelChoose();  //撤销已经选择的棋子
                    this->cancelFeasibility();  //撤销所有的可行性标志
                    this->board->update();              //重绘（允许在自己的客户端重绘）
                    this->board->client_send_MOVE();    //发送信号（里面包含了next函数）
                    return;
                }
            }
        }
    }
}

// 根据鼠标点击的位置得到当前点击的棋子对象
Coord& Room::getCoord(QPoint& curPoint)
{
    for (int i = 0; i <= 24; i++)     // 枚举行
    {
        for (int j = 0; j <= 16; j++)   // 枚举列
        {
            // 如果这个位置是有棋槽的
            if (this->coords[i][j].getHaveSlot() == true)
            {
                // distance是用户点击点和当前枚举的棋槽点的距离
                int distance =
                    (curPoint.x() - this->coords[i][j].coord2QPoint().x()) *
                    (curPoint.x() - this->coords[i][j].coord2QPoint().x()) +
                    (curPoint.y() - this->coords[i][j].coord2QPoint().y()) *
                    (curPoint.y() - this->coords[i][j].coord2QPoint().y());

                // 距离小于棋槽半径则返回引用
                if (distance <= SLOTR * SLOTR)
                {
//                    qDebug() << "you click coords:" << this->coords[i][j].coord_x << " " <<
//                             this->coords[i][j].coord_y << " " << "\n";
                    return this->coords[i][j];
                }
            }
        }
    }

    // 如果用户点击的位置在界面外，则返回nullCoord，届时通过比较地址进行校对
    return nullCoord;
}

void Room::clearRoom()
{
    // 将倒计时秒数设置为不会结束的范围
    this->countdown->setSecs(100000);
    this->countdown->start();
    this->countdown->setVisible(false);

    // qDebug() << "enter clearRoom1";
    if (historyRoom != nullptr)
    {
        this->historyRoom->originRoom = nullptr; // 反正下一次会重新设置new出来的historyRoom，所以不必作强制清除要求
    }
    this->board->isServer = true;   // clearRoom是服务端的函数，因此isServer必定是true

    // qDebug() << "enter clearRoom2";

    curRoundCnt = 1;
    mode = 3;                       // 因为一定是服务端，所以设置mode为3
    networkMode = true;            // 因为一定是服务端，所以设置networkMode为true
    userCnt = 0;                    //
    roomNumber = "";                //
    curChosenCoord = NOTCHOSEN;
    INIT_GAME = false;
    USER_HAS_BEEN_INIT = false;
    START_GAME = false;
    userCntInRoom = 0;
    mysystem->usedRoomCnt--;

    qDebug() << "enter clearRoom3";

    for (int i = 0; i <= 6; i++)
    {
        users[i].setUserColor(unull);
        users[i].id = 0;
        users[i].zoom = "O";
        users[i].userName = "";
        users[i].jumpTime = 0;
        users[i].isWin = false;
        users[i].isFail = false;
        users[i].socket = nullptr;
        playerList[i].isReady = false;
        playerList[i].userName = "";
        playerList[i].socket = nullptr;
        winnerList[i].isReady = false;
        winnerList[i].userName = "";
        winnerList[i].socket = nullptr;
    }

    qDebug() << "enter clearRoom4";

    this->initCoords();
    this->show();

    qDebug() << "leave clearRoom";
    return;
}
