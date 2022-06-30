#ifndef ROOM_H
#define ROOM_H

#include <QWidget>
#include <QPainter>
#include "board.h"
#include "hello.h"
#include "ask_server_or_client.h"
#include "ask_join_room.h"
#include "showinfo.h"
#include "predefine.h"

extern Coord NOTCHOSEN;

class QWHTimimg;

class ChooseRoom;

class ChooseRound;

class State
{
public :
    State();
    State(State & state);
    Coord coordsState[25][17];
    _userColor beforeWho = unull;
};

class Round
{
public:
    Round();
    State state[MAX_USERCNT];
    int roundNum = 1;
    int curStateCnt = 1;
};

namespace Ui
{
    class Room;
}

class Room : public QWidget
{
    Q_OBJECT
public:
    explicit Room(QWidget *parent = nullptr);
    ~Room();

    // 以下是一些调bug使用的 bool 变量
    bool DEBUG = true;  // 如果为true，进入debug模式

    // bool CURUSER_INFO = false;  // 如果为true，则输出当前房间行棋玩家的状态（是否获胜，是否被判负，玩家执棋颜色是什么）

    bool MOVE_TRACK_INFO = false;  // 如果为true，则输出当前房间行棋玩家的行棋轨迹

    // bool ROOM_STATE_INFO = false;   // 如果为true则输出房间的状态（为空、为满、是否开始游戏...）

    // bool ROOM_PLAYER_INFO = false;  // 如果为true则输出当前房间的所有玩家信息

    // bool COORD_COLOR_INFO[7] = {false, false, false, false, false, false, false};

    ChooseRound *chooseRound;

    ChooseRoom *chooseRoom;

    Room *historyRoom;

    Room *originRoom;

    bool IS_HISTORY_ROOM = false;

    //回合计数
    int curRoundCnt = 1;       // 从第一回合开始级数

    Round round[MAX_ROUND + 1];   // 该变量只有在historyRoom中才会用到，优化！

    // 现在要做的所有工作就是省略上面那一行代码，优化效率，增大可开的房间数量

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

    Coord curChosenCoord = NOTCHOSEN;

    /* 这是决定是否初始化游戏的变量，整个游戏进程中只需要进行一次，所以采用以下特殊的方式进行设置
     * ywt:2022\4\16 虽然我不知道下面前天的这两行代码具体是写来干嘛的，现在看起来第二个bool变量似乎没什么用
     * 但是既然程序运行流畅，建议就不要再乱改了......
     * hello出现的时候是false，hello关闭以后设置为true，画完以后再改为false
     */
    bool INIT_GAME = false;

    bool USER_HAS_BEEN_INIT = false;

    bool START_GAME = false;   // 该值在初始化时为false，在client_receive_START_TURN和server_receive_PLAYER_READY之后的修改为true

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

    virtual void mouseReleaseEvent(QMouseEvent *event);

    Coord& getCoord(QPoint& curPoint);

public slots:

    void showNext();

    void hideNext();

    void showChangeRoom();

    void hideChangeRoom();

    void showHistory();

    void hideHistory();

    /**
     * @brief chooseThisRoom 该函数用于选择并转换到这个房间
     */
    void chooseThisRoom();

    /**
     * @brief hideCanOrNot 隐藏选择复盘的按钮
     */
    void hideCanOrNot();

    /**
     * @brief showCanOrNot 显示选择复盘的按钮
     */
    void showCanOrNot();

    /**
     * @brief initWidget 该函数用于初始化房间中的控件
     */
    void initWidget();

    /**
     * @brief buildHistoryRoom 构建历史房间，初始化历史房间的一些性质
     * @return 返回的是构造好的房间
     */
    Room* buildHistoryRoom();

    /**
     * @brief callHistoryRoom 调用历史房间，并跳转到指定的轮次
     * @param hisRound 轮次
     * @param hisUserColor 在哪名用户之前
     */
    void callHistoryRoom(int hisRound, _userColor hisUserColor);

    /**
     * @brief clickHistory 点击历史，应该弹出chooseRound
     */
    void clickHistory();

    /**
     * @brief changeRooundState 该函数用于将房间中的轮数和状态移动到指定的状态上
     * @param hisRound 指定的轮数
     * @param hisUserColor 指定的状态（在哪名用户之前）
     */
    void changeRoundState(int hisRound, _userColor hisUserColor);

    // 重写绘图函数（room的绘图函数）
    virtual void paintEvent(QPaintEvent *event);

    // 桌面背景的绘制应该是room的行为
    void setTVBackground();

    // 轮次统计和记录每一次的棋盘状态
    void recordState();

    // coord的计算原理:id和Coord的转换
    void initCoords();

    void initUsers();

    /**
     * @brief initRound 该函数应该在initCoord之后调用
     */
    void initRound();

    /**
     * @brief initGame 初始化一局游戏
     */
    void initGame();

    //next函数用来转移操作权和当前操作用户
    void next();

    void nextImag(QPainter &painter);

    void search();
    void cancelChoose();
    void cancelFeasibility();
    void choose(Coord& chosenCoord);

    //历史复现函数
    void you_can_you_up();

    void clearRoom();

private:
    Ui::Room *ui;
};

#endif // ROOM_H
