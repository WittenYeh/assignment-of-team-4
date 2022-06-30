#ifndef CHOOSEROUND_H
#define CHOOSEROUND_H

#include <QWidget>

#include "room.h"

namespace Ui
{
    class ChooseRound;
}

class ChooseRound : public QWidget
{
    Q_OBJECT
public:
    explicit ChooseRound(QWidget *parent = nullptr);
    ~ChooseRound();
    Room* inWhichRoom;

public slots:
    void clickConfirm();

private:
    Ui::ChooseRound *ui;
};

#endif // CHOOSEROUND_H
