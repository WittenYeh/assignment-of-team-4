#ifndef CHOOSEROOM_H
#define CHOOSEROOM_H

#include <QWidget>
#include "room.h"

namespace Ui
{
    class ChooseRoom;
}

class ChooseRoom : public QWidget
{
    Q_OBJECT
public:
    explicit ChooseRoom(QWidget *parent = nullptr);
    ~ChooseRoom();
    Room* inWhichRoom;
    void clickConfirm();

private:
    Ui::ChooseRoom *ui;
};

#endif // CHOOSEROOM_H
