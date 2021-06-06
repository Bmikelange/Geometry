#include "iterator_on_vertices.h"

Iterator_on_vertices::Iterator_on_vertices()
{
    index=0;
}


Iterator_on_vertices  Iterator_on_vertices::operator++(int x)
{
    Iterator_on_vertices value;
    index++;
    value.index=index;
    value._vertices=_vertices;
    return value;
}

Vertice &  Iterator_on_vertices::operator*()
{
    return (*_vertices)[index];
}

bool Iterator_on_vertices::operator==(Iterator_on_vertices i)
{
    return index == i.index;
}

bool Iterator_on_vertices::operator!=(Iterator_on_vertices i)
{
    return index != i.index;
}
