#ifndef SYSTEM_H
#define SYSTEM_H
#include "cp-networkdata.h"
#include "cp-networksocket.h"
#include "cp-networkserver.h"
#include "room.h"

class System: public QWidget
{
    Q_OBJECT
public:

    int usedRoomCnt = 0; //已经被使用的房间(非空的房间)

    NetworkServer *server;

    NetworkSocket *socket;

    Room room[MAX_ROOMCNT + 1];

    bool REQUEST_INFO = true;  // 如果为true则输出当前房间收到的所有请求信息

    Room *curRoom;   // 当前房间

    // 下面三个变量是异常房间
    Room *nullRoom;   // 一个空房间，当所有房间都已经满时，返回这个房间指针
    Room *fullRoom;   // 一个满房间，当这个房间中已经充满人时，返回这个房间指针
    Room *sameNameRoom;  // 一个名字出现重复的异常房间
    Room *runningRoom; // 一个正在运行的房间
    Room *notInRoom;   // 当前玩家不在任何房间内时返回的异常房间

    Room historyRoom[11];   // 历史房间

    ask_server_or_client *ask1;

    ask_join_room *ask2;

    ShowInfo *showInfo;

    Hello *hello;

    System();

    /**
     * @brief findClientRoom 通过socket来找到客户端位于什么房间
     * @param socket 客户端的socket
     * @return 返回值应该是一个房间指针，是客户端所在房间的指针
     */
    Room* findClientRoom(QTcpSocket *socket);

public slots:
    /**
     * @brief receive 用来实现对NetworkData的接收和解析
     * @param data NetworkData对象
     */
    void receive(NetworkData data);

    void receiveData(QTcpSocket *socket, NetworkData data);

    void initWidgets();

    void dealLeave(QTcpSocket* client);

    void initRooms();
    /**
     * @brief introduceRoom 该函数用来为客户端寻找合适的房间
     * @param aroomNumber  该参数是客户端用户输入的房间名
     * @return 返回值应该是为客户端找到的房间
     */
    Room* introduceRoom(NetworkData data);
};

#endif // SYSTEM_H
