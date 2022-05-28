#include "system.h"
#include "hello.h"
#include "board.h"
#include "ui_hello.h"
#include "ask_server_or_client.h"
#include "qwhtimimg.h"

extern System *mysystem;
// extern int  userCnt;
// extern int  mode;
// extern bool INIT_GAME;
// extern bool USER_HAS_BEEN_INIT;
// extern ask_server_or_client *ask1;
// extern Board* board;

Hello::Hello(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hello)
{
    ui->setupUi(this);
    connect(ui->a_single_mode,
            &QPushButton::clicked,
            this,
            &Hello::set_a_single_mode);
    connect(ui->AI_mode, &QPushButton::clicked, this, &Hello::set_AI_mode);
    connect(ui->network_mode,
            &QPushButton::clicked,
            this,
            &Hello::set_network_mode);
}

Hello::~Hello()
{
    delete ui;
}

void Hello::get_userCnt()
{
    QString suserCnt = ui->lineEdit->text();
    qDebug() << "suserCnt is " << suserCnt;
    QString str2("2");
    QString str3("3");
    QString str6("6");

    if (suserCnt == str2)
    {
        mysystem->curRoom->userCnt = 2;
    }

    if (suserCnt == str3)
    {
        mysystem->curRoom->userCnt = 3;
    }

    if (suserCnt == str6)
    {
        mysystem->curRoom->userCnt = 6;
    }

    if (suserCnt == nullptr) // 什么都不写，默认为6
    {
        mysystem->curRoom->userCnt = 6;
    }
}

void Hello::set_a_single_mode()
{
    mysystem->curRoom->mode = 1;
    get_userCnt();
    mysystem->curRoom->INIT_GAME = true;
    mysystem->curRoom->initUsers();
    mysystem->curRoom->USER_HAS_BEEN_INIT = true;

    mysystem->curRoom->board->setGeometry(200, 50, 1200, 750);

    mysystem->curRoom->board->show();

    this->close();
}

void Hello::set_AI_mode()
{
    mysystem->curRoom->mode = 2;
    get_userCnt();
    mysystem->curRoom->INIT_GAME = true;
    mysystem->curRoom->initUsers();
    mysystem->curRoom->USER_HAS_BEEN_INIT = true;
    this->close();
}

void Hello::set_network_mode()
{
    mysystem->curRoom->mode = 3;
    // get_userCnt();
    // INIT_GAME = true;
    // initUsers();
    // USER_HAS_BEEN_INIT = true;
    this->close();

    mysystem->ask1 = new ask_server_or_client();

    //显示ask1窗口
    mysystem->ask1->show();
}


