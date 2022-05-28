#ifndef ROOM_H
#define ROOM_H

#include "board.h"
#include "hello.h"
#include "ask_server_or_client.h"
#include "ask_join_room.h"
#include "showinfo.h"

class QWHTimimg;

class Room
{
public:
    Room();
    Room *historyRoom;

    int mode;  // 1，2，3代表单人,AI,联机

    bool networkMode;  // 0代表客户端

    // 表示当前游戏的允许加入人数，有2，3，6三个选项
    int userCnt = 0;
    // 表示当前房间已经进入的人数(包括玩家自身)

    // 表示当前的房间号
    QString roomNumber;

    QWHTimimg *countdown;
    //棋盘
    Board *board;

    Coord curChosenCoord;

    /* 这是决定是否初始化游戏的变量，整个游戏进程中只需要进行一次，所以采用以下特殊的方式进行设置
     * ywt:2022\4\16 虽然我不知道下面前天的这两行代码具体是写来干嘛的，现在看起来第二个bool变量似乎没什么用
     * 但是既然程序运行流畅，建议就不要再乱改了......
     * hello出现的时候是false，hello关闭以后设置为true，画完以后再改为false
     */
    bool INIT_GAME = false;
    bool USER_HAS_BEEN_INIT = false;
    bool START_GAME = false;

    User* curUser;
    //因为再初始化me的时候我们还不知道一局游戏中有多少玩家
    //这个时候正好可以利用一下users的保留位users[0],先赋值me的信息然后等收到服务端发送的玩家信息再进行信息传递
    //坐标
    Coord coords[25][17];

    User users[7];  //这些users在客户端和服务端会分别获得不同程度的应用

    int userCntInRoom = 0;//在客户端，userCntInRoom用来绘制玩家列表，在服务端，userCntInRoom用来判断房间是否满员

    //表示等待进入房间的玩家列表
    Player playerList[7];
    //表示已经获得胜利的玩家列表
    Player winnerList[7];

    void initCoords();
    void initUsers();

    //next函数用来转移操作权和当前操作用户
    void next();
    void search();
    void cancelChoose();
    void cancelFeasibility();
    void choose(Coord& chosenCoord);
};

#endif // ROOM_H
