#include "board.h"
#include "hello.h"
#include "ask_server_or_client.h"
#include "ask_join_room.h"
#include "showinfo.h"
#include "qwhtimimg.h"
#include "system.h"

#include <QApplication>

QString myRoomNumber;

//表示用户输入的名字
QString myName;

System *mysystem;

Hello *hello;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // qDebug() << "main1";

    mysystem = new System();

    mysystem->initRooms();

    mysystem->initWidgets();

    mysystem->show();

    mysystem->hello->show();

    return a.exec();
}
