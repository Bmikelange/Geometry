#ifndef ITERATOR_ON_FACES_H
#define ITERATOR_ON_FACES_H

#include "utilitaire.h"
#include "vector"

class Iterator_on_faces
{
public:
    int index;
    std::vector<Face> * _faces;
    Iterator_on_faces();
    Iterator_on_faces  operator++(int x);
    Face & operator*();
    bool operator==(Iterator_on_faces i);
    bool operator!=(Iterator_on_faces i);
};

#endif // ITERATOR_ON_FACES_H
