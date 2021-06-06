#ifndef CIRCULATOR_ON_FACES_H
#define CIRCULATOR_ON_FACES_H
#include "vector"
#include "utilitaire.h"

class Circulator_on_faces
{
public:
    int indexFaces;
    int indexVertices;
    std::vector<Vertice> * _vertices;
    std::vector<Face> * _faces;
    Circulator_on_faces();
    Circulator_on_faces operator++(int x);
    Face  & operator*();
    bool operator==(Circulator_on_faces i);
    bool operator!=(Circulator_on_faces i);
    int searchnextFaces(int i, int j,int m);
};

#endif // CIRCULATOR_ON_FACES_H
