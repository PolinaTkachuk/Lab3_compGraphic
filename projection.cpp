#include "projection.h"
#include <QPainter>

Projection::Projection()
{
    //ограничиваем размер формы
    setFixedSize(QSize(900, 500));

    //добавляем карандаш
    setPensil();
    resetMatrix();

    //создаем таймер
    timer = new QTimer();
    //сиигнал-слот вращение вокруг ОУ
    connect(timer, SIGNAL(timeout()), this, SLOT(rotateByY()));
}

void Projection::startRotate()//начало вращения
{
    timer->start(10);
}

void Projection::reset()//перезагрузка
{
    if(timer->isActive())
        timer->stop();//остановка таймера

    resetMatrix(); //и обнуление матриц
}

//Правило приведения матрицы тела к центру тяжести
//Если AXс + BYс + CZс < 0, то изменить знаки коэффициентов A, B, C
//на пртивоположные.

//вращение по ОХ
void Projection::rotateByX()
{
    //resetMatrix();
    //приводим матрицу к центру тяжести с противоп знаками
    matrix.shift(-centerPensil.getX(), -centerPensil.getY(), -centerPensil.getZ());
    matrix.rotateByX(3.14/(2*200));
    matrix.shift(centerPensil.getX(), centerPensil.getY(), centerPensil.getZ());

    update();
}

void Projection::rotateByY()
{
    //resetMatrix();
     //приводим матрицу к центру тяжести с противоп знаками
    matrix.shift(-centerPensil.getX(), -centerPensil.getY(), -centerPensil.getZ());
    matrix.rotateByY(3.14/(2*200));
    matrix.shift(centerPensil.getX(), centerPensil.getY(), centerPensil.getZ());
    update();
}

void Projection::rotateByZ()
{
    //resetMatrix();
     //приводим матрицу к центру тяжести с противоп знаками
    matrix.shift(-centerPensil.getX(), -centerPensil.getY(), -centerPensil.getZ());
    matrix.rotateByZ(3.14/(2*200));
    matrix.shift(centerPensil.getX(), centerPensil.getY(), centerPensil.getZ());
    update();
}

void Projection::setPensil() //рисуем карандаш
{
    centerPensil = Point3D(40, 0, 0);

    pensil = Figure(13);

    //задняя грань карандаша
    pensil.setPoint(0, Point3D(0,-4,-6));
    pensil.setPoint(1, Point3D(0,4,-6));
    pensil.setPoint(2, Point3D(0,8,0));
    pensil.setPoint(3, Point3D(0,4,6));
    pensil.setPoint(4, Point3D(0,-4,6));
    pensil.setPoint(5, Point3D(0,-8,0));

    //соединяем линиями задний 6-угольник
    for(int i = 1; i < 6; i++)
        pensil.connectPoint(i-1, i); //соединение точек
    pensil.connectPoint(0, 5);//соединение точек

    //передняя грань карандаша
    for(int i = 0; i < 6; i++)
    {
        Point3D newPoint = pensil.getPoint(i);
        newPoint.shiftByX(64);
        pensil.setPoint(i+6, newPoint);
    }

    //соединяем линиями передний 6-угольник
    for(int i = 7; i < 12; i++)
        pensil.connectPoint(i-1, i);
    pensil.connectPoint(6, 11);

    //соединяем линиями соответствующие вершины 6-угольников
    for(int i = 0; i < 6; i++)
        pensil.connectPoint(i, i+6);

    //делаем вершину кончика карандаша
    pensil.setPoint(12, Point3D(80,0,0));
    for(int i = 6; i < 12; i++)
        pensil.connectPoint(i, 12);


    //точка зрения
    pointOfView = Point3D(80,0,0);
}

void Projection::resetMatrix()
{
    matrix.reset();

    update();
}

void Projection::paintEvent(QPaintEvent *event)// прорисовка событий
{
    Figure Figure2D;
    Figure2D = pensil;
    //ИЗМЕНЕНИЕ ТОЧЕК В МАТРИЦЕ У ФИГУРЫ НА КАЖДОМ ШАГЕ
    Figure2D.changeFigure(matrix);

    //РАСЧЕТ МАТРИЦЫ ТЕЛА и определение лицевых граней
    Figure2D.СalcMatrixBody(Point3D(0, 0, 100));


    Matrix matrix2D;
    //матр сдвигв точку зрения
    matrix2D.shift(-pointOfView.getX(), -pointOfView.getY(), -pointOfView.getZ());
    matrix2D.scale(5, 5, 5);
    matrix2D.shift(1280/2, 720/2, 0); //сдвигаем на полуразмеры экрана

    Figure2D.changeFigure(matrix2D);

    //ПРОРИСОВКА

    QPainter painter(this);

    //прорисовка видимых и невидимых граней
    for (int i = 0; i < Figure2D.getCountPoints(); i++)
        for (int j = i + 1; j < Figure2D.getCountPoints(); j++)
            if (Figure2D.checkLine(i, j)) //если грань существует
            {   if(Figure2D.checkViewedLine(i, j))//если грань видимая
                {

                    //прорисовываем ее зеленым
                    //добавление карандаша

                    painter.setPen(QPen(QBrush(Qt::green), 3));

                    //прорисовка грани
                    painter.drawLine(QLine(QPoint(Figure2D[i].getX(), Figure2D[i].getY()), QPoint(Figure2D[j].getX(), Figure2D[j].getY())));
                }
                else//иначе если грань не видимая, каранадш красный
                    painter.setPen(QPen(QBrush(Qt::red), 1));
                //не прорисовываем грань
                //painter.drawLine(QLine(QPoint(Figure2D[i].getX(), Figure2D[i].getY()), QPoint(Figure2D[j].getX(), Figure2D[j].getY())));
            }
            //else//проверка ( несуществующую грань нельзя прорисовать)
           // {
               // if (Figure2D.checkViewedLine(i, j))
              //  {
                //    painter.setPen(QPen(QBrush(Qt::red), 3));
                //    painter.drawLine(QLine(QPoint(Figure2D[i].getX(), Figure2D[i].getY()), QPoint(Figure2D[j].getX(), Figure2D[j].getY())));
               // }

           // }

}

