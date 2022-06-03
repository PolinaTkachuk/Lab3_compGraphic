#ifndef PROJECTION_H
#define PROJECTION_H

#include <QWidget>
#include <QTimer>
#include "figure.h"

class Projection : public QWidget
{
    Q_OBJECT
public:
    Projection();
    ~Projection() { delete timer; }

    QTimer *timer;//таймер
    Point3D centerPensil; //центр тяжести  тела
    Figure pensil;//
    Point3D pointOfView;// точка зрения
    Matrix matrix;// матрица

public slots:
    void startRotate();
    void reset();
private slots:
    void rotateByX();
    void rotateByY();
    void rotateByZ();
    void resetMatrix();
    void setPensil();
    void paintEvent(QPaintEvent *event);
};

#endif // PROJECTION_H
