#include "teminal.h"
#include "ui_teminal.h"
#include "predefine.h"
#include <iostream>

extern System *mysystem;

Teminal::Teminal(QWidget *parent) : QWidget(parent), ui(new Ui::Teminal)
{
    ui->setupUi(this);
    // this->setAttribute(Qt::WA_TranslucentBackground, true);
    connect(this->ui->confirm, &QPushButton::clicked, this, &Teminal::getCommand);
    // this->ui->scrollArea->setBackgroundRole(QPalette::Dark);  // 背景色
}

Teminal::~Teminal()
{
    delete ui;
}

void Teminal::getCommand()
{
    QString command = this->ui->command->text();
    // 先将命令按空格分开
    QString command_op = command.section(" ", 0, 0);
    QString command_content = command.section(" ", 1, 1);
    if (command_op == "debug")
    {
        if (command_content == "all" )
        {
            for (int i = 1; i <= MAX_ROOMCNT; i++)
            {
                mysystem->room[i].DEBUG = !mysystem->room[i].DEBUG;
            }
            qDebug() << "debug mode all rooms has beed openned";
        }
        else
        {
            if (command_content == "")
            {
                return;
            }
            for (int i = 1; i <= MAX_ROOMCNT; i++)
            {
                if (mysystem->room[i].roomNumber == command_content)
                {
                    mysystem->room[i].DEBUG = !mysystem->room[i].DEBUG;
                    qDebug() << "debug mode of " << mysystem->room[i].roomNumber << " has been openned";
                }
            }

        }
    }

    if (command_op == "curuser")
    {
        if (command_content == "")
        {
            return;
        }
        for (int i = 1; i <= MAX_ROOMCNT; i++)
        {
            if (mysystem->room[i].roomNumber == command_content)
            {
                qDebug() << "curUser of room " << mysystem->room[i].roomNumber << "is " << mysystem->room[i].curUser->userName;
            }
        }
    }

    if (command_op == "movetrack")
    {
        if (command_content == "")
        {
            return;
        }
        for (int i = 1; i <= MAX_ROOMCNT; i++)
        {
            if (mysystem->room[i].roomNumber == command_content)
            {
                mysystem->room[i].MOVE_TRACK_INFO = !mysystem->room[i].MOVE_TRACK_INFO;
                if (mysystem->room[i].MOVE_TRACK_INFO)
                {
                    qDebug() << "movetrack mode of " << mysystem->room[i].roomNumber << " has been openned";
                }
                else
                {
                    qDebug() << "movetrack mode of " << mysystem->room[i].roomNumber << " has been closed";
                }
            }
        }
    }

    if (command_op == "request")
    {
        mysystem->REQUEST_INFO = !mysystem->REQUEST_INFO;
        if (mysystem->REQUEST_INFO)
        {
            qDebug() << "request_info moede has been openned";
        }
        else
        {
            qDebug() << "request_info mode has been closed";
        }
    }

    if (command_op == "roomstate")
    {
        if (command_content == "")
        {
            qDebug() << "the following room has been initialed";
            for (int i = 1; i <= MAX_ROOMCNT; i++)
            {
                if (mysystem->room[i].roomNumber != "")
                {
                    qDebug() << "i = " << i << ": " << mysystem->room[i].roomNumber;
                }
            }
            return;
        }
        else
        {
            for (int i = 1; i <= MAX_ROOMCNT; i++)
            {
                if (mysystem->room[i].roomNumber == command_content)
                {
                    qDebug() << "the room " << mysystem->room[i].roomNumber << "'s info:";
                    qDebug() << "running state:" << mysystem->room[i].START_GAME;
                    qDebug() << "full state:" << (mysystem->room[i].userCntInRoom == 6);
                    qDebug() << "curRoundCnt: " << mysystem->room[i].curRoundCnt;
                    qDebug() << "userCntInRoom； " << mysystem->room[i].userCntInRoom;
                }
            }
        }
    }

    if (command_op == "player")
    {
        if (command_content == "")
        {
            return;
        }
        else
        {
            for (int i = 1; i <= MAX_ROOMCNT; ++i)
            {
                if (mysystem->room[i].roomNumber == command_content)
                {
                    qDebug() << "the player information of " << mysystem->room[i].roomNumber << ":";
                    for (int j = 1; j <= mysystem->room[i].userCntInRoom; ++j)
                    {
                        qDebug() << mysystem->room[i].playerList[j].userName << ": " << "isReady(" << mysystem->room[i].playerList[j].isReady << ")";
                    }
                }
            }
        }
    }

    if (command_op == "user")
    {
        if (command_content == "")
        {
            return;
        }
        else
        {
            for (int i = 1; i <= MAX_ROOMCNT; ++i)
            {
                if (mysystem->room[i].roomNumber == command_content)
                {
                    qDebug() << "the player information of " << mysystem->room[i].roomNumber << ":";
                    for (int j = 1; j <= 6; ++j)
                    {
                        if (mysystem->room[i].users[j].userName != "")
                        {
                            qDebug() << mysystem->room[i].users[j].userName << "(" << int(mysystem->room[i].users[j].getUserColor()) << ")" << ": " << "isFail:" << mysystem->room[i].users[j].isFail << "; isWin:" << mysystem->room[i].users[j].isWin;
                        }
                    }
                }
            }
        }
    }

    if (command_op == "winner")
    {
        if (command_content == "")
        {
            return;
        }
        else
        {
            for (int i = 1; i <= MAX_ROOMCNT; ++i)
            {
                if (mysystem->room[i].roomNumber == command_content)
                {
                    qDebug() << "the player information of " << mysystem->room[i].roomNumber << ":";
                    for (int j = 1; j <= 6; ++j)
                    {
                        if (mysystem->room[i].winnerList[j].userName != "")
                        {
                            qDebug() << mysystem->room[i].winnerList[j].userName;
                        }
                    }
                }
            }
        }
    }

    if (command_op == "yellow")
    {
        if (command_content == "")
        {
            return;
        }
        else
        {
            for (int i = 1; i <= MAX_ROOMCNT; ++i)
            {
                if (mysystem->room[i].roomNumber == command_content)
                {
                    qDebug() << "all yellow coords are as followed:";

                    for (int j = 0; j <= 24; ++j)
                    {
                        for (int k = 0; k <= 16; ++k)
                        {
                            if (mysystem->room[i].coords[j][k].getColor() == yellow)
                            {
                                std::cout << "(" << mysystem->room[i].coords[j][k].coord_x << "," << mysystem->room[i].coords[j][k].coord_y << ") ";
                            }
                        }
                    }

                    qDebug() << "yellow coords' target is:";
                    for (int j = 0; j <= 24 ; ++j)
                    {
                        for (int k = 0 ; k <= 24; ++k)
                        {
                            if (mysystem->room[i].coords[j][k].targetColor == yellow)
                            {
                                std::cout << "(" << mysystem->room[i].coords[j][k].coord_x << "," << mysystem->room[i].coords[j][k].coord_y << ") ";
                            }
                        }
                    }
                    std::cout << std::endl;  // 等全部输出结束以后再追加一个换行符
                }
            }
        }
    }

    if (command_op == "red")
    {
        if (command_content == "")
        {
            return;
        }
        else
        {
            for (int i = 1; i <= MAX_ROOMCNT; ++i)
            {
                if (mysystem->room[i].roomNumber == command_content)
                {
                    qDebug() << "all red coords are as followed:";

                    for (int j = 0; j <= 24; ++j)
                    {
                        for (int k = 0; k <= 16; ++k)
                        {
                            if (mysystem->room[i].coords[j][k].getColor() == red)
                            {
                                std::cout << "(" << mysystem->room[i].coords[j][k].coord_x << "," << mysystem->room[i].coords[j][k].coord_y << ") ";
                            }
                        }
                    }
                    std::cout << std::endl;  // 等全部输出结束以后再追加一个换行符
                }
            }
        }
    }

    if (command_op == "purple")
    {
        if (command_content == "")
        {
            return;
        }
        else
        {
            for (int i = 1; i <= MAX_ROOMCNT; ++i)
            {
                if (mysystem->room[i].roomNumber == command_content)
                {
                    qDebug() << "all purple coords are as followed:";

                    for (int j = 0; j <= 24; ++j)
                    {
                        for (int k = 0; k <= 16; ++k)
                        {
                            if (mysystem->room[i].coords[j][k].getColor() == purple)
                            {
                                std::cout << "(" << mysystem->room[i].coords[j][k].coord_x << "," << mysystem->room[i].coords[j][k].coord_y << ") ";
                            }
                        }
                    }
                    std::cout << std::endl;  // 等全部输出结束以后再追加一个换行符
                }
            }
        }
    }

    if (command_op == "blue")
    {
        if (command_content == "")
        {
            return;
        }
        else
        {
            for (int i = 1; i <= MAX_ROOMCNT; ++i)
            {
                if (mysystem->room[i].roomNumber == command_content)
                {
                    qDebug() << "all blue coords are as followed:";

                    for (int j = 0; j <= 24; ++j)
                    {
                        for (int k = 0; k <= 16; ++k)
                        {
                            if (mysystem->room[i].coords[j][k].getColor() == blue)
                            {
                                std::cout << "(" << mysystem->room[i].coords[j][k].coord_x << "," << mysystem->room[i].coords[j][k].coord_y << ") ";
                            }
                        }
                    }
                    std::cout << std::endl;  // 等全部输出结束以后再追加一个换行符
                }
            }
        }
    }

    if (command_op == "cyan")
    {
        if (command_content == "")
        {
            return;
        }
        else
        {
            for (int i = 1; i <= MAX_ROOMCNT; ++i)
            {
                if (mysystem->room[i].roomNumber == command_content)
                {
                    qDebug() << "all cyan coords are as followed:";

                    for (int j = 0; j <= 24; ++j)
                    {
                        for (int k = 0; k <= 16; ++k)
                        {
                            if (mysystem->room[i].coords[j][k].getColor() == cyan)
                            {
                                std::cout << "(" << mysystem->room[i].coords[j][k].coord_x << "," << mysystem->room[i].coords[j][k].coord_y << ") ";
                            }
                        }
                    }
                    std::cout << std::endl;  // 等全部输出结束以后再追加一个换行符
                }
            }
        }
    }

    if (command_op == "green")
    {
        if (command_content == "")
        {
            return;
        }
        else
        {
            for (int i = 1; i <= MAX_ROOMCNT; ++i)
            {
                if (mysystem->room[i].roomNumber == command_content)
                {
                    qDebug() << "all green coords are as followed:";

                    for (int j = 0; j <= 24; ++j)
                    {
                        for (int k = 0; k <= 16; ++k)
                        {
                            if (mysystem->room[i].coords[j][k].getColor() == green)
                            {
                                std::cout << "(" << mysystem->room[i].coords[j][k].coord_x << "," << mysystem->room[i].coords[j][k].coord_y << ") ";
                            }
                        }
                    }
                    std::cout << std::endl;  // 等全部输出结束以后再追加一个换行符
                }
            }
        }
    }

    if (command_op == "curroom")
    {
        if (mysystem->curRoom->roomNumber != "")
        {
            qDebug() << "curroom is room " << mysystem->curRoom->roomNumber;
        }
        else
        {
            qDebug() << "curroom is null";
        }
    }
}
