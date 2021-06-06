#ifndef CIRCULATOR_ON_VERTICES_H
#define CIRCULATOR_ON_VERTICES_H
#include "vector"
#include "utilitaire.h"

class Circulator_on_vertices
{
public:
    int indexVertices;
    int indexFaces;
    int indiceVertex;
    std::vector<Vertice> * _vertices;
    std::vector<Face> * _faces;
    Circulator_on_vertices();
    Circulator_on_vertices operator++(int x);
    Vertice  & operator*();
    bool operator==(Circulator_on_vertices i);
    bool operator!=(Circulator_on_vertices i);
    int searchnextVertice(int i, int j,int m);
};

#endif // CIRCULATOR_ON_VERTICES_H
