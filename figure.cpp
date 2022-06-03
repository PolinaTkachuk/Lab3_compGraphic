#include "figure.h"

void sorting(int& first, int& second)//сортировка, (большая координата,меньшая)
{
    if (first < second)
    {
        int temp = first;
        first = second;
        second = temp;
    }
}

Figure::Figure(int countPoints_)
{
//выделяем память
    countPoints = countPoints_;
    Points = new Point3D[countPoints];
    matrixLines = new bool*[countPoints];
    matrixViewedLines = new bool*[countPoints];


    for (int i = 0; i < countPoints; i++)
    {
        matrixLines[i] = new bool[i+1];
        matrixViewedLines[i] = new bool[i+1];
    }

    //заполяем матрицы 0
    for(int i = 0; i < countPoints; i++)
        for(int j = 0; j < (i+1); j++)
        {
            matrixLines[i][j] = 0;
            matrixViewedLines[i][j] = 0;
        }
}

Figure::~Figure()
{
    if(countPoints != 0)
    {
        delete[] Points;

        for (int i = 0; i < countPoints; i++)
        {
            delete [] matrixLines[i];
            delete [] matrixViewedLines[i];
        }

        delete[] matrixLines;
        delete[] matrixViewedLines;
    }
}

void Figure::setPoint(int index, Point3D newPoint)//добавление точки
{
    Points[index] = newPoint;
}

Point3D Figure::getPoint(int index) //возвращение точки
{
    return Points[index];
}

void Figure::connectPoint(int indexFirstPoint, int indexSecondPoint)
 //провести грань между точками
{
    if (indexFirstPoint < countPoints && indexFirstPoint >= 0 && indexSecondPoint < countPoints && indexSecondPoint >= 0)
    {
        //отсортир по индексам
        sorting(indexFirstPoint, indexSecondPoint);
        //признак существования грани
        matrixLines[indexFirstPoint][indexSecondPoint] = true;
    }
}

void Figure::disconnectPoint(int indexFirstPoint, int indexSecondPoint)
//не провести грань между точками
{
    if (indexFirstPoint < countPoints && indexFirstPoint >= 0 && indexSecondPoint < countPoints && indexSecondPoint >= 0)
    {
        sorting(indexFirstPoint, indexSecondPoint);
         //признак НЕ существования грани
        matrixLines[indexFirstPoint][indexSecondPoint] = false;
    }
}

bool Figure::checkLine(int indexFirstPoint, int indexSecondPoint)
//проверка на существование грани
{
    if (!matrixLines)//признак существования грани
        return false;

    sorting(indexFirstPoint, indexSecondPoint);

    return matrixLines[indexFirstPoint][indexSecondPoint];
}

bool Figure::checkViewedLine(int indexFirstPoint, int indexSecondPoint)
 //проверка на существование видимой грани
{
    if (!matrixViewedLines)
        return false;

    sorting(indexFirstPoint, indexSecondPoint);
    return matrixViewedLines[indexFirstPoint][indexSecondPoint];
}

void Figure::СalcMatrixBody(Point3D pointView)
//расчет матрицы тела+ определение нелицевых граней относительно pointView-переданной точки зрения
{
    //центр тяжести тела
    Point3D centerFigure;
    //Формулы для вычисления центра тяжести тела
    for (int i = 0; i < countPoints; i++)
    {
        centerFigure.shiftByX(Points[i].getX()/countPoints);
        centerFigure.shiftByY(Points[i].getY()/countPoints);
        centerFigure.shiftByZ(Points[i].getZ()/countPoints);
    }

    //заполняем карандаш
    int** planePoints = new int*[13];
    for(int i = 0; i < 13; i++)
        planePoints[i] = new int[3];
    //задняя грань карандаша
    planePoints[0][0] = 0;
    planePoints[0][1] = 1;
    planePoints[0][2] = 2;

    //грани основной части карандаша
    for (int i = 1; i < 7; i++)
    {
        planePoints[i][0] = i-1;
        planePoints[i][1] = i;
        planePoints[i][2] = i-1+6;
    }
    planePoints[6][1] = 0;

    //грани кончика карандаша
    for (int i = 7; i < 13; i++)
    {
        planePoints[i][0] = i-1;
        planePoints[i][1] = i;
        planePoints[i][2] = 12;
    }
    planePoints[12][1] = 6;


    //РАСЧЕТ матрицы тела
    double** W = new double*[13];
    for (int i = 0; i < 13; i++)
    {
        W[i] = new double[4];
        W[i][0] = (Points[planePoints[i][2]].getY() - Points[planePoints[i][0]].getY()) *
                (Points[planePoints[i][1]].getZ() - Points[planePoints[i][0]].getZ()) -
                (Points[planePoints[i][1]].getY() - Points[planePoints[i][0]].getY()) *
                (Points[planePoints[i][2]].getZ() - Points[planePoints[i][0]].getZ());

        W[i][1] = (Points[planePoints[i][1]].getX() - Points[planePoints[i][0]].getX()) *
                (Points[planePoints[i][2]].getZ() - Points[planePoints[i][0]].getZ()) -
                (Points[planePoints[i][2]].getX() - Points[planePoints[i][0]].getX()) *
                (Points[planePoints[i][1]].getZ() - Points[planePoints[i][0]].getZ());

        W[i][2] = (Points[planePoints[i][2]].getX() - Points[planePoints[i][0]].getX()) *
                (Points[planePoints[i][1]].getY() -Points[planePoints[i][0]].getY()) -
                (Points[planePoints[i][1]].getX() - Points[planePoints[i][0]].getX()) *
                (Points[planePoints[i][2]].getY() - Points[planePoints[i][0]].getY());

        W[i][3] = -(W[i][0]*Points[planePoints[i][0]].getX() + W[i][1]*Points[planePoints[i][0]].getY()+
                W[i][2]*Points[planePoints[i][0]].getZ());
    }

    // Для каждой грани проверьте, не является ли она нелицевой?

    //задний 6-угольник
    if(W[0][0]*pointView.getX() + W[0][1]*pointView.getY() + W[0][2]*pointView.getZ() < 0)
    {
        for(int i = 1; i < 6; i++)
            matrixViewedLines[i][i-1] = true;
        matrixViewedLines[5][0] = true;
    }

    //боковые грани карандаша
    for(int i = 1; i < 6; i++)
        if (W[i][0]*pointView.getX() + W[i][1]*pointView.getY() + W[i][2]*pointView.getZ() >= 0)
        {
            matrixViewedLines[i][i-1] = true;
            matrixViewedLines[i-1 + 6][i-1] = true;
            matrixViewedLines[i + 6][i] = true;
            matrixViewedLines[i + 6][i-1 + 6] = true;
        }
    if (W[6][0]*pointView.getX() + W[6][1]*pointView.getY() + W[6][2]*pointView.getZ() >= 0)
    {
        matrixViewedLines[5][0] = true;
        matrixViewedLines[11][5] = true;
        matrixViewedLines[6][0] = true;
        matrixViewedLines[11][6] = true;
    }

    //грани кончика карандаша
    for(int i = 7; i < 12; i++)
        if (W[i][0]*pointView.getX() + W[i][1]*pointView.getY() + W[i][2]*pointView.getZ() >= 0)
        {
            matrixViewedLines[i][i-1] = true;
            matrixViewedLines[12][i-1] = true;
            matrixViewedLines[12][i] = true;
        }
    if (W[12][0]*pointView.getX() + W[12][1]*pointView.getY() + W[12][2]*pointView.getZ() >= 0)
    {
        matrixViewedLines[11][6] = true;
        matrixViewedLines[12][6] = true;
        matrixViewedLines[12][11] = true;
    }

}


void Figure::changeFigure(Matrix matrix)
{
    for (int i = 0; i < countPoints; i++)
        Points[i] = matrix.changePoint(Points[i]);
}

void Figure::operator =(const Figure& temp)
{
    if (&temp != this)
    {
        delete[] Points;

        for (int i = 0; i < countPoints; i++)
            delete[] matrixLines[i];
        delete[] matrixLines;

        for (int i = 0; i < countPoints; i++)
            delete[] matrixViewedLines[i];
        delete[] matrixViewedLines;

        countPoints = temp.countPoints;

        Points = new Point3D[countPoints];
        for (int i = 0; i < countPoints; i++)
            Points[i] = temp.Points[i];

        matrixLines = new bool*[countPoints];
        for (int i = 0; i < countPoints; i++)
        {
            matrixLines[i] = new bool[i+1];
            for (int j = 0; j < i+1; j++)
                matrixLines[i][j] = temp.matrixLines[i][j];
        }

        matrixViewedLines = new bool*[countPoints];
        for (int i = 0; i < countPoints; i++)
        {
            matrixViewedLines[i] = new bool[i+1];
            for (int j = 0; j < i+1; j++)
                matrixViewedLines[i][j] = temp.matrixViewedLines[i][j];
        }
    }
}
