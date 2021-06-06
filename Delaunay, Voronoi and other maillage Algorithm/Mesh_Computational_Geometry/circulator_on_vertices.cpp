#include "circulator_on_vertices.h"
#include <QDebug>

Circulator_on_vertices::Circulator_on_vertices()
{
    indexVertices = 0;
    indexFaces = 0;
    indiceVertex = 0;
}

int Circulator_on_vertices::searchnextVertice(int i, int j,int m)
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

Circulator_on_vertices Circulator_on_vertices::operator++(int x)
{
    Face f=(*_faces)[indexFaces];
    Circulator_on_vertices c;
    int indice_second_vertex;
    if(f.f1==indexVertices)
        indice_second_vertex=f.f3;
    if(f.f2==indexVertices)
        indice_second_vertex=f.f1;
    if(f.f3==indexVertices)
        indice_second_vertex=f.f2;
    indexFaces=searchnextVertice(indexVertices,indice_second_vertex,indexFaces);
    indiceVertex = indice_second_vertex;
    c.indexVertices=indexVertices;
    c.indexFaces=indexFaces;
    c.indiceVertex = indiceVertex;
    c._faces=_faces;
    c._vertices=_vertices;
    return c;

}

bool Circulator_on_vertices::operator==(Circulator_on_vertices i)
{
    return indiceVertex==i.indiceVertex;
}
bool Circulator_on_vertices::operator!=(Circulator_on_vertices i)
{
    return indiceVertex!=i.indiceVertex;
}

Vertice & Circulator_on_vertices:: operator*()
{
    return (*_vertices)[indiceVertex];
}
