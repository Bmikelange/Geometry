#include "circulator_on_faces.h"
#include <QDebug>

Circulator_on_faces::Circulator_on_faces()
{
    indexFaces=0;
    indexVertices=0;
}

int Circulator_on_faces::searchnextFaces(int i, int j,int m)
{
    Face f1=(*_faces)[(*_faces)[m].i1];
    Face f2=(*_faces)[(*_faces)[m].i2];
    Face f3=(*_faces)[(*_faces)[m].i3];
    if(f1.contain(i,j))
        return (*_faces)[m].i1;
    if(f2.contain(i,j))
        return (*_faces)[m].i2;
    if(f3.contain(i,j))
        return (*_faces)[m].i3;
    return 0;
}


Circulator_on_faces Circulator_on_faces::operator++(int x)
{
    Face f=(*_faces)[indexFaces];
    Circulator_on_faces c;
    int indice_second_vertex;
    if(f.f1==indexVertices)
        indice_second_vertex=f.f3;
    if(f.f2==indexVertices)
        indice_second_vertex=f.f1;
    if(f.f3==indexVertices)
        indice_second_vertex=f.f2;
    indexFaces=searchnextFaces(indexVertices,indice_second_vertex,indexFaces);

    c.indexVertices=indexVertices;
    c.indexFaces=indexFaces;
    c._faces=_faces;
    c._vertices=_vertices;
    return c;
}


Face  & Circulator_on_faces::operator*()
{
    return (*_faces)[indexFaces];
}
bool Circulator_on_faces::operator==(Circulator_on_faces i)
{
    return indexFaces==i.indexFaces;
}

bool Circulator_on_faces::operator!=(Circulator_on_faces i)
{
    return indexFaces!=i.indexFaces;
}
