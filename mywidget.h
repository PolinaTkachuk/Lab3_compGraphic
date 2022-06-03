#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include "projection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mywidget; }
QT_END_NAMESPACE

class mywidget : public QWidget
{
    Q_OBJECT

public:
    mywidget(QWidget *parent = nullptr);//конструктор
    ~ mywidget();//деструктор

    QVBoxLayout *generalVLayout;
    QHBoxLayout *buttonHLayout;

    QPushButton *startButton;
    QPushButton *resetButton;

    Projection *myProjection;
private:
   // Ui::mywidget *ui;
};
#endif // MYWIDGET_H
