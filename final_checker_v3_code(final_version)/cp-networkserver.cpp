#include "cp-networkserver.h"
#include <QDebug>
#include <iostream>
#include <system.h>
#include <room.h>

extern System *mysystem;

NetworkServer::NetworkServer(QObject* parent)
    : QTcpServer(parent)
    , disconnMapper(new QSignalMapper(this))
    , recvMapper(new QSignalMapper(this))
{

    this->listen(QHostAddress::Any, 16666);

    connect(this, &QTcpServer::newConnection, this, &NetworkServer::newconnection);
    connect(this->disconnMapper, &QSignalMapper::mappedObject, this, &NetworkServer::disconnect);
    connect(this->recvMapper, &QSignalMapper::mappedObject, this, &NetworkServer::receiveData);
}

void NetworkServer::receiveData(QObject* obj)
{
    QTcpSocket* client = dynamic_cast<QTcpSocket*>(obj);
    Q_ASSERT(client != nullptr);
    QByteArray whole_block = client->readAll();
    QList<QByteArray> blocks = whole_block.split('\n');

    Room* tempRoom = mysystem->findClientRoom(client);
    QString tempUserName;
    for (int i = 1; i <= tempRoom->userCntInRoom; ++i)
    {
        if (client == tempRoom->playerList[i].socket)
        {
            tempUserName = tempRoom->playerList[i].userName;
        }
    }

    for (const auto& block : blocks)
    {
        if (block.isEmpty())
        {
            continue;
        }
        try
        {
            NetworkData data(block);
            qDebug() << "Server: " << tempRoom->roomNumber << "(" << tempUserName << ")";
            QString data_op;
            switch (data.op)
            {
                case (OPCODE::CLOSE_ROOM_OP):
                    data_op = "CLOSE_ROOM_OP";
                    break;
                case (OPCODE::END_GAME_OP):
                    data_op = "END_GAME_OP";
                    break;
                case (OPCODE::END_TURN_OP):
                    data_op = "END_TURN_OP";
                    break;
                case (OPCODE::ERROR_OP):
                    data_op = "ERROR_OP";
                    break;
                case (OPCODE::JOIN_ROOM_OP):
                    data_op = "JOIN_ROOM_OP";
                    break;
                case (OPCODE::JOIN_ROOM_REPLY_OP):
                    data_op = "JOIN_ROOM_REPLY_OP";
                    break;
                case (OPCODE::LEAVE_ROOM_OP):
                    data_op = "LEAVE_ROOM_OP";
                    break;
                case (OPCODE::MOVE_OP):
                    data_op = "MOVE_OP";
                    break;
                case (OPCODE::PLAYER_READY_OP):
                    data_op = "PLAYER_READY_OP";
                    break;
                case (OPCODE::START_GAME_OP):
                    data_op = "START_GAME_OP";
                    break;
                default:
                    break;
            }
            qDebug() << data_op << ' ' << data.data1 << ' ' << data.data2 << Qt::endl;
            emit receive(client, data);
        }
        catch (const InvalidMessage& m)
        {
            emit parseError(m);
        }
    }
}

void NetworkServer::send(QTcpSocket* client, NetworkData data)
{
    QString data_op;
    switch (data.op)
    {
        case (OPCODE::CLOSE_ROOM_OP):
            data_op = "CLOSE_ROOM_OP";
            break;
        case (OPCODE::END_GAME_OP):
            data_op = "END_GAME_OP";
            break;
        case (OPCODE::END_TURN_OP):
            data_op = "END_TURN_OP";
            break;
        case (OPCODE::ERROR_OP):
            data_op = "ERROR_OP";
            break;
        case (OPCODE::JOIN_ROOM_OP):
            data_op = "JOIN_ROOM_OP";
            break;
        case (OPCODE::JOIN_ROOM_REPLY_OP):
            data_op = "JOIN_ROOM_REPLY_OP";
            break;
        case (OPCODE::LEAVE_ROOM_OP):
            data_op = "LEAVE_ROOM_OP";
            break;
        case (OPCODE::MOVE_OP):
            data_op = "MOVE_OP";
            break;
        case (OPCODE::PLAYER_READY_OP):
            data_op = "PLAYER_READY_OP";
            break;
        case (OPCODE::START_GAME_OP):
            data_op = "START_GAME_OP";
            break;
        case (OPCODE::START_TURN_OP):
            data_op = "START_TURN_OP";
            break;
        default:
            break;
    }

    if (client == nullptr)
    {
        qDebug() << "WARN: this client has been cleaned";
    }

    Room* tempRoom = mysystem->findClientRoom(client);
    QString tempUserName;
    for (int i = 1; i <= tempRoom->userCntInRoom; ++i)
    {
        if (client == tempRoom->playerList[i].socket)
        {
            tempUserName = tempRoom->playerList[i].userName;
        }
    }
    qDebug() << "send " << data_op << data.data1 << data.data2 << " to " << tempRoom->roomNumber << "(" << tempUserName << ")" << Qt::endl;
    client->write(data.encode());
    client->flush();
}

void NetworkServer::disconnect(QObject* client)
{
    for (auto it = clients.begin(); it != clients.end(); ++it)
    {
        if (*it == client)
        {
            emit leave(*it);
            clients.erase(it);
        }
        if (it == clients.end())
        {
            break;
        }
    }
}

void NetworkServer::newconnection()
{
    QTcpSocket* client = this->nextPendingConnection();
    clients.append(client);

    qDebug() << "newconnection";

    connect(client, &QIODevice::readyRead, this->recvMapper, qOverload<>(&QSignalMapper::map));
    connect(client, &QAbstractSocket::disconnected, this->disconnMapper, qOverload<>(&QSignalMapper::map));
    this->disconnMapper->setMapping(client, client);
    this->recvMapper->setMapping(client, client);
}
