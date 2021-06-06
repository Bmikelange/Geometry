#ifndef LAPLACIAN_H
#define LAPLACIAN_H
#include "mesh.h"
#include "iterator_on_vertices.h"
#include "circulator_on_vertices.h"
#include "utilitaire.h"

class laplacian
{

public:
    Circulator_on_vertices cironv;
    Iterator_on_vertices itonv;
    std::vector<Point> laplacians;
    Mesh mesh;
    laplacian();
    laplacian(Mesh m):mesh(m){}
    void computeLaplacian();
    int searchnextFace(int i, int j,int m);
    void drawMeshlap();
};

#endif // LAPLACIAN_H
