#ifndef TEMINAL_H
#define TEMINAL_H

#include <QWidget>
#include <qlabel.h>
#include <QScrollArea>
#include <system.h>

namespace Ui
{
    class Teminal;
}

class Teminal : public QWidget
{
    Q_OBJECT
public:
    explicit Teminal(QWidget *parent = nullptr);
    ~Teminal();
    // QScrollArea *scroll;
public slots:
    void getCommand();

private:
    Ui::Teminal *ui;
    int rowNum;
};

#endif // TEMINAL_H
