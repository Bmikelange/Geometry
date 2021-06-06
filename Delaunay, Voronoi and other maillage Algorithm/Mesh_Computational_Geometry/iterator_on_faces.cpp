#include "iterator_on_faces.h"

Iterator_on_faces::Iterator_on_faces()
{
    index =0;
}

Iterator_on_faces Iterator_on_faces::operator++(int x)
{
    Iterator_on_faces value;
    index++;
    value.index=index;
    value._faces=_faces;
    return value;
}
Face &  Iterator_on_faces::operator*()
{
    return (*_faces)[index];
}

bool Iterator_on_faces::operator==(Iterator_on_faces i)
{
    return index == i.index;
}

bool Iterator_on_faces::operator!=(Iterator_on_faces i)
{
    return index != i.index;
}
