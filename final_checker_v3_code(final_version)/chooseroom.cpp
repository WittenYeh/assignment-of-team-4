#include "chooseroom.h"
#include "ui_chooseroom.h"

#include "system.h"
extern System* mysystem;

ChooseRoom::ChooseRoom(QWidget *parent) : QWidget(parent), ui(new Ui::ChooseRoom)
{
    ui->setupUi(this);
    this->setVisible(false);

    connect(ui->confirm, &QPushButton::clicked, this, &ChooseRoom::clickConfirm);
    connect(ui->exit, &QPushButton::clicked, this, &ChooseRoom::close);

    // 设置鼠标图标
    QCursor cursor ;
    QPixmap pixmap(MOUSE) ;
    cursor = QCursor(pixmap, 0, 0);
    setCursor(cursor) ;
}

void ChooseRoom::clickConfirm()
{
    QString tempRoomNumber = this->ui->enterRoomNumber->text();
    for (int i = 1; i <= MAX_ROOMCNT; i++)
    {
        if (tempRoomNumber == mysystem->room[i].roomNumber)
        {
            mysystem->room[i].chooseThisRoom();
            break;
        }
    }
    this->close();
}

ChooseRoom::~ChooseRoom()
{
    delete ui;
}
