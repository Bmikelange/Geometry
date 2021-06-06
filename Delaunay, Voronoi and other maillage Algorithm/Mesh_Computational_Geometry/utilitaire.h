#include <cmath>

#ifndef UTILITAIRE_H
#define UTILITAIRE_H


class Point
{
public:
    double _x;
    double _y;
    double _z;

    Point():_x(),_y(),_z() {}
    Point(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_) {}
};


class Vertice
{
public:
    Point p1;
    int i1;
    int index;
    Vertice(Point _p1,int _i1,int _index):p1(_p1),i1(_i1),index(_index){}
    Vertice(){p1=Point(0,0,0);i1=-1;index=-1;}
};

class Face
{
public:
    int f1,f2,f3;
    int i1,i2,i3;
    Face(int _p1,int _p2,int _p3,int _i1,int _i2,int _i3):f1(_p1),f2(_p2),f3(_p3),i1(_i1),i2(_i2),i3(_i3){}
    bool contain(int x,int y);
};

#endif // UTILITAIRE_H
