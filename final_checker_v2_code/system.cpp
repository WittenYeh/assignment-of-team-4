#include "system.h"
#include "room.h"
#include "predefine.h"
#include <iostream>

System::System()
{
    // 初始化房间
    this->curRoom = &room[1];

    this->nullRoom = new Room();
    nullRoom->setVisible(false);
    this->notInRoom = new Room();
    notInRoom->setVisible(false);
    this->runningRoom = new Room();
    runningRoom->setVisible(false);
    this->sameNameRoom = new Room();
    sameNameRoom->setVisible(false);

    // 设置房间的可视状态
    for (int i = 1; i <= 10; i++)
    {
        this->room[i].setVisible(false);
    }
    this->curRoom->setVisible(true);

    //在窗口类的构造函数中加入下列代码来完成初始化和信号与槽的连接
    this->server = new NetworkServer(this);

    connect(this->server, &NetworkServer::receive, this, &System::receiveData);

    //初始化socket
    this->socket = new NetworkSocket(new QTcpSocket(), this);

    connect(socket, &NetworkSocket::receive, this, &System::receive);

    //初始化信号和槽的断开的操作
    connect(socket->base(), &QAbstractSocket::disconnected, [ = ]()
    {
        QMessageBox::critical(this, tr("Connection lost"), tr("Connection to server has closed"));
    });

    //连接失败(连接失败是只有在客户端才会出现的反馈)
    connect(socket->base(), &QAbstractSocket::errorOccurred, this->curRoom->board, &Board::displayError);

    //连接成功(连接成功也是只有在客户端才会出现的反馈)
    connect(socket->base(), &QAbstractSocket::connected, this->curRoom->board, &Board::connected);

    // 处理client直接点击退出的情况
    connect(server, &NetworkServer::leave, this, &System::dealLeave);

    this->setGeometry(0, 0, 1300, 780);

    // qDebug() << "system 8";

    // 设置鼠标图标
    QCursor cursor ;
    QPixmap pixmap(MOUSE) ;
    cursor = QCursor(pixmap, 0, 0);
    setCursor(cursor) ;
}

//槽函数receive，用来实现对NetworkData的接收和解析,这是一个非常长的函数,整合了各种函数和操作
void System::receive(NetworkData data)
{
    //错误处理操作符：在整个进程中随时监听
    if (data.op == OPCODE::ERROR_OP)
    {
        //将错误码发送给showinfo对象进行处理
        emit this->curRoom->board->client_receive_ERROR(data.data1, data.data2);
    }

    //处理服务端发送的加入房间结果的反馈
    //收到这个消息代表已经加入成功
    if (data.op == OPCODE::JOIN_ROOM_REPLY_OP)
    {
        this->curRoom->board->client_receive_JOIN_ROOM_REPLY(data);
    }

    if (data.op == OPCODE::JOIN_ROOM_OP)
    {
        this->curRoom->board->client_receive_JOIN_ROOM(data);
    }

    if (data.op == OPCODE::LEAVE_ROOM_OP)
    {
        this->curRoom->board->client_receive_LEAVE_ROOM(data);
    }

    if (data.op == OPCODE::PLAYER_READY_OP)
    {
        this->curRoom->board->client_receive_PLAYER_READY(data);
    }

    if (data.op == OPCODE::START_GAME_OP)
    {
        this->curRoom->board->client_receive_START_GAME(data);
    }

    if (data.op == OPCODE::START_TURN_OP)
    {
        this->curRoom->board->client_receive_START_TURN(data);
    }

    if (data.op == OPCODE::MOVE_OP)
    {
        this->curRoom->board->client_receive_MOVE(data);
    }

    if (data.op == OPCODE::END_TURN_OP)
    {
        this->curRoom->board->client_receive_END_TURN(data);
    }

    if (data.op == OPCODE::END_GAME_OP)
    {
        this->curRoom->board->client_receive_END_GAME(data);
    }
}

Room *System::findClientRoom(QTcpSocket *socket)
{
    for (int i = 1; i <= MAX_ROOMCNT; i++)
    {
        for (int j = 1; j <= 6; j++)
        {
            if (this->room[i].playerList[j].socket != nullptr)
            {
                if (this->room[i].playerList[j].socket == socket)
                {
                    // () << "room[" << i << "]" << " get the information";
                    return &room[i];   // 找到玩家所处的房间并返回
                }
            }
        }
    }
    return this->notInRoom;   // 如果玩家不在任何房间内，返回一个异常房间
}

void System::receiveData(QTcpSocket * socket, NetworkData data)
{
    Room *tempRoom = findClientRoom(socket);

//    if (tempRoom->roomNumber != "")
//    {
//        qDebug() << "by room " << tempRoom->roomNumber << ": ";
//    }
//    else
//    {
//        std::cout << "unknown room: ";
//    }

    if (data.op == OPCODE::JOIN_ROOM_OP)  // 这里不能禁止notInRoom的使用，而是将notInRoom作为一个临时的站，使用它的introduce来提供真正的房间
    {
        tempRoom->board->server_receive_JOIN_ROOM(socket, data);
    }

    if (data.op == OPCODE::PLAYER_READY_OP)
    {
        if (tempRoom == this->notInRoom) // 如果当前发来的消息用户不在任何房间内
        {
            tempRoom->board->server_send_ERROR(socket, "Sorry, you are not in any of the rooms");
            return;
        }
        tempRoom->board->server_receive_PLATER_READY(socket, data);
    }

    if (data.op == OPCODE::LEAVE_ROOM_OP)
    {
        if (tempRoom == this->notInRoom) // 如果当前发来的消息用户不在任何房间内
        {
            tempRoom->board->server_send_ERROR(socket, "Sorry, you are not in any of the rooms");
            return;
        }
        tempRoom->board->server_receive_LEAVE_ROOM(socket, data);
    }

    if (data.op == OPCODE::MOVE_OP)
    {
        if (tempRoom == this->notInRoom) // 如果当前发来的消息用户不在任何房间内
        {
            tempRoom->board->server_send_ERROR(socket, "Sorry, you are not in any of the rooms");
            return;
        }
        tempRoom->board->server_receive_MOVE(socket, data);
    }
}

void System::initWidgets()
{
    // 初始化子控件，设置这些子控件的父窗口
    this->ask1 = new ask_server_or_client();
    this->ask1->setVisible(false);
    this->ask1->setParent(this);

    this->ask2 = new ask_join_room();
    this->ask2->setVisible(false);
    this->ask2->setParent(this);

    this->hello = new Hello();
    this->hello->setVisible(false);
    this->hello->setParent(this);

    this->showInfo = new ShowInfo();
    this->showInfo->setVisible(false);
    this->showInfo->setParent(this);
}

void System::initRooms()
{
    for (int i = 1; i <= MAX_ROOMCNT; i++)
    {
        this->room[i].setParent(this);
    }
}

void System::dealLeave(QTcpSocket* client)
{
    Room* tempRoom = this->findClientRoom(client);  // 找到离开的用户位于哪个房间
    if (tempRoom->START_GAME == true)
    {
        return;  // 如果该房间游戏已经开始就不必打扰，因为会有函数在对局结束后清空房间，这间房间中的玩家也就清空了，不会残留
    }
    else
    {
        // 如果服务端自己有相关错误强制退出处理机制，我们应该避免重新调用一次server_receive_MOVE_OP
        // 该功能尚不启用（如果某个客户端添加了处理机制再启用）
//        if (tempRoom == this->notInRoom)
//        {
//            return;
//        }

        QString leavingUserName;
        for (int i = 1; i <= 6; i++)     // 在房间中寻找离开的用户是谁
        {
            if (tempRoom->playerList[i].socket == client)  // 找到离开的用户
            {
                leavingUserName = tempRoom->playerList[i].userName;  // 将leavingUserName设置为离开用户的名字
            }
        }
        tempRoom->board->server_receive_LEAVE_ROOM(client, NetworkData(OPCODE::LEAVE_ROOM_OP, leavingUserName, ""));  // 该用户所在的房间接收离开请求
    }
}

Room* System::introduceRoom(NetworkData data)
{
    // 注：data1是房间号，data2是用户名
    // 查找当前用户输入的房间名是不是某间房间的名字
    // qDebug() << "the room name is " << data.data1;
    // qDebug() << "the user nmae is " << data.data2;
    for (int i = 1; i <= MAX_ROOMCNT; i++)
    {
        if (data.data1 == this->room[i].roomNumber)   // 如果用户输入的房间名已经存在
        {
            if (room[i].userCntInRoom == 6)   // 房间是否为满的检查
            {
                qDebug() << "return fullRoom";
                return this->fullRoom;
            }
            if (room[i].START_GAME == true)  // 房间是否正在游戏的检查
            {
                qDebug() << "return runningRoom";
                return this->runningRoom;
            }
            for (int j = 1; j <= 6; j++)  // 人名重复检查
            {
                if (this->room[i].playerList[j].userName == data.data2)  //重名
                {
                    qDebug() << "return sameNameRoom";
                    return this->sameNameRoom;
                }
            }
            return &room[i];   // 如果通过检查，则返回这间房间
        }
    }
    // 如果房间已经全部开满，则直接返回空房间
    if (this->usedRoomCnt == MAX_ROOMCNT)
    {
        qDebug() << "return nullRoom";
        return this->nullRoom;
    }
    // 如果房间没有全部开满，则返回下标最小的空房间
    else
    {
        for (int i = 1; i <= MAX_ROOMCNT; ++i)
        {
            if (room[i].roomNumber == "")  // 前提： 不仅要满足房间号最小，还要满足房间名为空，并且已经使用的房间数事实上不一定能代表最新的房间
            {
                usedRoomCnt++;  //已经使用的房间数+1
                qDebug() << "return new Room of " << i;
                this->room[i].roomNumber = data.data1;  // 设置房间号
                return &this->room[i];
            }
        }
    }
}

