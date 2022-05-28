#include "board.h"
#include "hello.h"
#include "ask_server_or_client.h"
#include "ask_join_room.h"
#include "showinfo.h"
#include "qwhtimimg.h"
#include "system.h"

#include <QApplication>

// 1代表单人模式，2代表AI模式，3代表联机模式
// int mode;

// 表示当前游戏的允许加入人数，有2，3，6三个选项
//int userCnt = 0;

// 表示当前房间已经进入的人数(包括玩家自身)
//int userCntInRoom = 0;//在客户端，userCntInRoom用来绘制玩家列表，在服务端，userCntInRoom用来判断房间是否满员

// 表示网络模式，0代表Client,1代表Server
// bool networkMode;

// 表示当前的房间号
//QString roomNumber;

QString myRoomNumber;

//表示用户输入的名字
QString myName;

//表示等待进入房间的玩家列表
//Player playerList[7];

//表示已经获得胜利的玩家列表
//Player winnerList[7];

/*TODO(ywt:2022/4/30)
 * 在绘图函数中增加一个判断语句，要求如果是联网模式的话，将有一个地方能用来显示房间号
 */
//Board *board;

System *mysystem;

Hello *hello;

//ask_server_or_client *ask1;

//ask_join_room *ask2;

//ShowInfo *showInfo;

//QWHTimimg *countdown;

// extern User users[7];

// 初始化me指针的函数是client_receive_START_GAME
// extern User* me;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug()<<"hhh1";

    mysystem = new System();

    qDebug()<<"hhh2";

    hello = new Hello();

    //所有board的子部件都被设置为不可见，只有在必要的时候才会开启

    // countdown->setVisible(false);

    // board->show();

    qDebug()<<"hhh3";

    hello->show();

    return a.exec();
}
