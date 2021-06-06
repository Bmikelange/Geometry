#ifndef ITERATOR_ON_VERTICES_H
#define ITERATOR_ON_VERTICES_H

#include "utilitaire.h"
#include "vector"

class Iterator_on_vertices
{
public:
    int index;
    std::vector<Vertice>  * _vertices;
    Iterator_on_vertices();
    Iterator_on_vertices operator++(int x);
    Vertice  & operator*();
    bool operator==(Iterator_on_vertices i);
    bool operator!=(Iterator_on_vertices i);
};

#endif // ITERATOR_ON_VERTICES_H
