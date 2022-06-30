#include "chooseround.h"
#include "ui_chooseround.h"
#include "predefine.h"

ChooseRound::ChooseRound(QWidget *parent) : QWidget(parent), ui(new Ui::ChooseRound)
{
    ui->setupUi(this);
    this->setVisible(false);

    connect(ui->confirm, &QPushButton::clicked, this, &ChooseRound::clickConfirm);
    connect(ui->exit, &QPushButton::clicked, this, &ChooseRound::close);

    // 设置鼠标图标
    QCursor cursor ;
    QPixmap pixmap(MOUSE) ;
    cursor = QCursor(pixmap, 0, 0);
    setCursor(cursor) ;
}

ChooseRound::~ChooseRound()
{
    delete ui;
}

void ChooseRound::clickConfirm()
{
    // 获取信息
    int whichRound = this->ui->whichRound->text().toInt();
    _userColor beforeWho = unull;
    if (this->ui->beforeWho->text() == "yellow")
    {
        beforeWho = uyellow;
    }
    else if (this->ui->beforeWho->text() == "red")
    {
        beforeWho = ured;
    }
    else if (this->ui->beforeWho->text() == "blue")
    {
        beforeWho = ublue;
    }
    else if (this->ui->beforeWho->text() == "purple")
    {
        beforeWho = upurple;
    }
    else if (this->ui->beforeWho->text() == "green")
    {
        beforeWho = ugreen;
    }
    else if (this->ui->beforeWho->text() == "cyan")
    {
        beforeWho = ucyan;
    }
    this->inWhichRoom->callHistoryRoom(whichRound, beforeWho);   // 调用历史房间
}
