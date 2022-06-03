#ifndef FIGURE_H
#define FIGURE_H

#include "point3d.h"
#include "matrix.h"

class Figure
{
private:
    int countPoints = 0;//кол-во вершин n
    Point3D* Points = nullptr;//массив точек фигуры
    bool** matrixLines = nullptr;//матрица грани (существует ли данная грань)
    bool** matrixViewedLines = nullptr;//матрица видимости грани (явл ли грань видимой=true)

public:
    Figure() {};
    Figure(int countPoints_);
    ~Figure();

    int getCountPoints() { return countPoints; }

    void setPoint(int index, Point3D newPoint);
    Point3D getPoint(int index);

    //провести грань между точками
    void connectPoint(int indexFirstPoint, int indexSecondPoint);
    //не провести грань между точками
    void disconnectPoint(int indexFirstPoint, int indexSecondPoint);
    //проверка на существование грани
    bool checkLine(int indexFirstPoint, int indexSecondPoint);
    //проверка на существование видимой грани
    bool checkViewedLine(int indexFirstPoint, int indexSecondPoint);

    //расчет матрицы тела+ определение нелицевых граней относительно pointView-переданной точки зрения
    void СalcMatrixBody(Point3D pointView);

    void changeFigure(Matrix matrix);

    void operator =(const Figure& temp);
    Point3D operator [](int index) { return Points[index]; }
};

#endif // FIGURE_H
