#include "ask_server_or_client.h"
#include "ui_ask_server_or_client.h"
#include "ask_join_room.h"
#include "board.h"
#include "predefine.h"
#include "showinfo.h"
#include "system.h"
#include "teminal.h"

//定义在main函数中
// extern Board *board;
// extern int networkMode;
// extern ask_join_room *ask2;
// extern ShowInfo *showInfo;
extern System *mysystem;
QString IP;

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

    // 设置鼠标图标
    QCursor cursor ;
    QPixmap pixmap(MOUSE) ;
    cursor = QCursor(pixmap, 0, 0);
    setCursor(cursor) ;
}

ask_server_or_client::~ask_server_or_client()
{
    delete ui;
}

void ask_server_or_client::setClientMode()
{
    mysystem->curRoom->networkMode = 0;    //执行客户端模式

    mysystem->curRoom->board->isServer = mysystem->curRoom->networkMode;

    delete mysystem->server;

    this->setVisible(false);

    mysystem->showInfo->showCONNECTING();  //显示正在连接的信息

    IP = this->ui->IP->text();             // 输入服务端IP地址

    mysystem->socket->hello(IP, 16666);   //一旦点击ClientMode就发送连接请求
}

void ask_server_or_client::setServerMode()
{
    // 设置一些基本属性
    mysystem->showInfo = new ShowInfo();
    for (int i = 1; i <= 10; i++)
    {
        mysystem->room[i].networkMode = 1;    //执行服务端模式
        mysystem->room[i].mode = 3;
        mysystem->room[i].board->isServer = true;
    }
    delete mysystem->socket;

    mysystem->show();

    Teminal *teminal = new Teminal();

    teminal->show();
}

