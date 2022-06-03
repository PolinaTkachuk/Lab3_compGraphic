#ifndef POINT3D_H
#define POINT3D_H

class Point3D
{
private:
    int x = 0;
    int y = 0;
    int z = 0;

public:
    Point3D() {};
    Point3D(int x_, int y_, int z_){  x = x_; y = y_;z = z_;}

    int getX() const { return x; }
    int getY() const { return y; }
    int getZ() const { return z; }

    void shiftByX(int shift){ x += shift;}
    void shiftByY(int shift){ y += shift;}
    void shiftByZ(int shift){ z += shift;}
};


#endif // POINT3D_H
