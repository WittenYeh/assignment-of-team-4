#ifndef SYSTEM_H
#define SYSTEM_H
#include "cp-networkdata.h"
#include "cp-networksocket.h"
#include "cp-networkserver.h"
#include "room.h"
#include <QObject>

class System:public QObject
{
    Q_OBJECT
public:
    Room room[10];

    Room *curRoom = &room[0];

    Room historyRoom[10];

    ask_server_or_client *ask1;

    ask_join_room *ask2;

    ShowInfo *showInfo;

    System();

};

#endif // SYSTEM_H
