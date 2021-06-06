#include "utilitaire.h"

bool Face::contain(int x, int y)
{
    if((f1==x && f2==y) || (f2==x && f1==y) ||
       (f3==x && f2==y) || (f2==x && f3==y) ||
       (f1==x && f3==y) || (f3==x && f1==y))
        return true;
    return false;
}
