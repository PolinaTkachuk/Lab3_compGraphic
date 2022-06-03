#include "mywidget.h"
#include "ui_mywidget.h"

mywidget::mywidget(QWidget *parent)
    : QWidget(parent)
{

    myProjection = new Projection();

    generalVLayout = new QVBoxLayout();
    buttonHLayout = new QHBoxLayout();
    //задание кнопок
    startButton = new QPushButton("start");
    resetButton = new QPushButton("stop");

    buttonHLayout->addWidget(startButton);
    buttonHLayout->addWidget(resetButton);

    generalVLayout->addWidget(myProjection);
    generalVLayout->addLayout(buttonHLayout);
    startButton->setStyleSheet("font: bold;background-color: yellow;font-size: 28px;height: 40px;width: 120px;border-radius: 4px transparent;");
    resetButton->setStyleSheet("font: bold;background-color: yellow;font-size: 28px;height: 40px;width: 120px;border-radius: 4px transparent;");



    setLayout(generalVLayout);

    //сигналы-слоты для кнопок
    connect(startButton, SIGNAL(clicked()), myProjection, SLOT(startRotate()));
    connect(resetButton, SIGNAL(clicked()), myProjection, SLOT(reset()));
}

mywidget::~mywidget()
{
    delete myProjection;

    delete generalVLayout;
    delete buttonHLayout;

    delete startButton;
    delete resetButton;
}
