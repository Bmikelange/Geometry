#include "laplacian.h"
#include <cmath>
#include <QDebug>

laplacian::laplacian()
{
}

int laplacian::searchnextFace(int i, int j,int m)
{
    Face f1=(*cironv._faces)[(*cironv._faces)[m].i1];
    Face f2=(*cironv._faces)[(*cironv._faces)[m].i2];
    Face f3=(*cironv._faces)[(*cironv._faces)[m].i3];
    if(f1.contain(i,j))
        return (*cironv._faces)[m].i1;
    if(f2.contain(i,j))
        return (*cironv._faces)[m].i2;
    if(f3.contain(i,j))
        return (*cironv._faces)[m].i3;
    return 0;
}

Point crossl(Point A,Point B)
{
    return Point( A._y * B._z - A._z * B._y ,
                  A._z * B._x - A._x * B._z ,
                  A._x * B._y - A._y * B._x );
}

float dotl(Point A, Point B)
{
    return  A._x * B._x + A._y * B._y + A._z * B._z;
}

float lengthl(Point v)
{
    return std::sqrt(v._x * v._x + v._y * v._y + v._z * v._z);
}

float triangleArea(float x,float y,float z)
{
    float s=(x+z+y)/2;
    return std::sqrt(s*(s-x)*(s-y)*(s-z));
}

void facesearch(Face f, Circulator_on_vertices cironv, int & indice_second_vertex,int  & indice_third_vertex)
{
    if(f.f1==cironv.indexVertices)
    {
        indice_second_vertex=f.f2;
        indice_third_vertex=f.f3;
    }
    if(f.f2==cironv.indexVertices)
    {
        indice_second_vertex=f.f3;
        indice_third_vertex=f.f1;
    }
    if(f.f3==cironv.indexVertices)
    {
        indice_second_vertex=f.f1;
        indice_third_vertex=f.f2;
    }
}

void laplacian::computeLaplacian()
{
    std::vector<double> lap;
    while(itonv != mesh.vertice_past_the_end())
    {
        cironv=mesh.adjacent_vertices(*itonv);
        Circulator_on_vertices temporaire=cironv;
        Face f=(*cironv._faces)[cironv.indexFaces];
        int indice_second_vertex, indice_third_vertex;
        facesearch(f,cironv,indice_second_vertex,indice_third_vertex);
        Face f2=(*cironv._faces)[searchnextFace(indice_second_vertex,cironv.indexVertices,cironv.indexFaces)];
        Point x=(*cironv._vertices)[cironv.indexVertices].p1;
        Point y=(*cironv._vertices)[indice_second_vertex].p1;
        Point z=(*cironv._vertices)[indice_third_vertex].p1;
        Point P1((float)(x._x-z._x),(float)(x._y-z._y),(float)(x._z-z._z));
        Point P2((float)(y._x-z._x),(float)(y._y-z._y),(float)(y._z-z._z));
        float cosalpha=dotl(P1,P2)/(lengthl(P1)*lengthl(P2));
        float sinalpha=1-cosalpha*cosalpha;
        facesearch(f,cironv,indice_second_vertex,indice_third_vertex);
        x=(*cironv._vertices)[cironv.indexVertices].p1;
        y=(*cironv._vertices)[indice_second_vertex].p1;
        z=(*cironv._vertices)[indice_third_vertex].p1;
        Point P3((float)(x._x-z._x),(float)(x._y-z._y),(float)(x._z-z._z));
        Point P4((float)(y._x-z._x),(float)(y._y-z._y),(float)(y._z-z._z));
        float cosbeta=dotl(P3,P4)/(lengthl(P3)*lengthl(P4));
        float sinbeta=1-cosbeta*cosbeta;
        float laplacian=(cosalpha/sinalpha + cosbeta/sinbeta);
        Point p(laplacian*(x._x-y._x),laplacian*(x._y-y._y),laplacian*(x._z-y._z));
        cironv++;
        double area=triangleArea(lengthl(P1),lengthl(P2), lengthl(Point(x._x-y._x,x._y-y._y,x._z-y._z)));
        temporaire.indiceVertex=indice_second_vertex;
        while(cironv != temporaire)
        {
            Face f=(*cironv._faces)[cironv.indexFaces];
            int indice_second_vertex, indice_third_vertex;
            facesearch(f,cironv,indice_second_vertex,indice_third_vertex);
            Face f2=(*cironv._faces)[searchnextFace(indice_second_vertex,cironv.indexVertices,cironv.indexFaces)];
            Point x=(*cironv._vertices)[cironv.indexVertices].p1;
            Point y=(*cironv._vertices)[indice_second_vertex].p1;
            Point z=(*cironv._vertices)[indice_third_vertex].p1;
            Point P6((float)(x._x-z._x),(float)(x._y-z._y),(float)(x._z-z._z));
            Point P7((float)(y._x-z._x),(float)(y._y-z._y),(float)(y._z-z._z));
            float cosalpha=dotl(P6,P7)/(lengthl(P6)*lengthl(P7));
            float sinalpha=1-cosalpha*cosalpha;
            facesearch(f,cironv,indice_second_vertex,indice_third_vertex);
            x=(*cironv._vertices)[cironv.indexVertices].p1;
            y=(*cironv._vertices)[indice_second_vertex].p1;
            z=(*cironv._vertices)[indice_third_vertex].p1;
            Point P8((float)(x._x-z._x),(float)(x._y-z._y),(float)(x._z-z._z));
            Point P9((float)(y._x-z._x),(float)(y._y-z._y),(float)(y._z-z._z));
            float cosbeta=dotl(P8,P9)/(lengthl(P8)*lengthl(P9));
            float sinbeta=1-cosbeta*cosbeta;
            float laplacian=(cosalpha/sinalpha + cosbeta/sinbeta);
            p._x+=laplacian*(x._x-y._x);
            p._y+=laplacian*(x._y-y._y);
            p._z+=laplacian*(x._z-y._z);
            area+=triangleArea(lengthl(P6),lengthl(P7), lengthl(Point(x._x-y._x,x._y-y._y,x._z-y._z)));
            cironv++;
        }
        double ar=(2.0*area/3.0);
        lap.push_back(pow(fabs((double)p._x/ar+(double)p._y/ar+(double)p._z/ar),1));
        itonv++;
    }
    double max = *std::max_element(lap.begin(),lap.end());
    for(int i=0;i<lap.size();i++)
    {
        double x=(lap[i]/max);
        laplacians.push_back(Point(x,0,1-x));
    }
}

void glPointDraws(const Point & p) {
    glVertex3f(p._x, p._y, p._z);
}

void laplacian::drawMeshlap()
{
    Iterator_on_faces itonf=mesh.faces_begin();
    while(itonf != mesh.faces_past_the_end())
    {
        float v=(*itonf).f1;
        float v2=(*itonf).f2;
        float v3=(*itonf).f3;
        glBegin(GL_TRIANGLES);
        glColor3d(laplacians[v]._x*255,laplacians[v]._y*255,laplacians[v]._z*255);
        glPointDraws((*cironv._vertices)[v].p1);
        glColor3d(laplacians[v2]._x*255,laplacians[v2]._y*255,laplacians[v2]._z*255);
        glPointDraws((*cironv._vertices)[v2].p1);
        glColor3d(laplacians[v3]._x*255,laplacians[v3]._y*255,laplacians[v3]._z*255);
        glPointDraws((*cironv._vertices)[v3].p1);
        glEnd();
        itonf++;
    }
}
