#include "ask_server_or_client.h"
#include "ui_ask_server_or_client.h"
#include "ask_join_room.h"
#include "board.h"
#include "showinfo.h"
#include "system.h"

//定义在main函数中
// extern Board *board;
// extern int networkMode;
// extern ask_join_room *ask2;
// extern ShowInfo *showInfo;
extern System *mysystem;

ask_server_or_client::ask_server_or_client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ask_server_or_client)
{
    //设置窗口居中
    // QPoint globalPos = board->mapToGlobal(QPoint(0, 0)); //父窗口绝对坐标
    // int x = globalPos.x() + (board->width() - this->width()) / 2;//x坐标
    // int y = globalPos.y() + (board->height() - this->height()) / 2;//y坐标
    this->move(270, 150);//窗口移动

    ui->setupUi(this);

    //点击按钮设置模式
    connect(ui->Client, &QPushButton::clicked, this, &ask_server_or_client::setClientMode);
    connect(ui->Server, &QPushButton::clicked, this, &ask_server_or_client::setServerMode);

    //设置按钮的点击导致窗口的关闭
    connect(ui->Client, &QPushButton::clicked, this, &ask_server_or_client::close);
    connect(ui->Server, &QPushButton::clicked, this, &ask_server_or_client::close);
}

ask_server_or_client::~ask_server_or_client()
{
    delete ui;
}

void ask_server_or_client::setClientMode()
{
    mysystem->curRoom->networkMode = 0;    //执行客户端模式

    mysystem->curRoom->board->isServer = mysystem->curRoom->networkMode;

    for(int i=0;i<=9;i++){
        delete mysystem->room[i].board->server;   //如果是客户端模式，删除server指针
    }


    mysystem->ask2 = new ask_join_room();
    mysystem->ask2->setVisible(false);

    mysystem->showInfo = new ShowInfo();
    mysystem->showInfo->setVisible(false);

    //ask2->show();  只有在连接成功后才显示



    mysystem->showInfo->showCONNECTING();  //显示正在连接的信息

    mysystem->curRoom->board->socket->hello("10.46.46.49", 16666);   //一旦点击ClientMode就发送连接请求
}

void ask_server_or_client::setServerMode()
{
    for(int i=0;i<=9;i++){

        mysystem->room[i].networkMode = 1;    //执行服务端模式

        mysystem->room[i].board->isServer = mysystem->room[i].networkMode;

        delete mysystem->room[i].board->socket;  //如果是服务端模式，删除socket指针
    }
}

