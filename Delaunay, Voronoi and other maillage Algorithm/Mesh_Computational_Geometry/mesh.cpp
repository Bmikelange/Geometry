#include "mesh.h"
#include <fstream>
#include <string>
#include <QDebug>
#include <QDir>

GeometricWorld::GeometricWorld()
{
    double width=0.5, depth=0.6, height=0.8;
    _bBox.push_back(Point(-0.5*width,-0.5*depth,-0.5*height)); //0
    _bBox.push_back(Point(-0.5*width,0.5*depth,-0.5*height)); // 1
    _bBox.push_back(Point(0.5*width,-0.5*depth,-0.5*height)); // 2
    _bBox.push_back(Point(-0.5*width,-0.5*depth,0.5*height)); // 3
}

// The following functions could be displaced into a module OpenGLDisplayGeometricWorld that would include mesh.h

// Draw a Point
void glPointDraw(const Point & p) {
    glVertex3f(p._x, p._y, p._z);
}


//Example with a bBox
void GeometricWorld::draw() {
    glColor3d(1,0,0);
    glBegin(GL_TRIANGLES);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[1]);
    glPointDraw(_bBox[2]);
    glEnd();

    glColor3d(0,1,0);
    glBegin(GL_TRIANGLES);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[2]);
    glPointDraw(_bBox[3]);
    glEnd();

    glColor3d(0,0,1);
    glBegin(GL_TRIANGLES);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[3]);
    glPointDraw(_bBox[1]);
    glEnd();

    //glColor3d(1,1,0);
}

//Example with a wireframe bBox
void GeometricWorld::drawWireFrame() {
    glColor3d(0,1,0);
    glBegin(GL_LINE_STRIP);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[1]);
    glEnd();
    glColor3d(0,0,1);
    glBegin(GL_LINE_STRIP);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[2]);
    glEnd();
    glColor3d(1,0,0);
    glBegin(GL_LINE_STRIP);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[3]);
    glEnd();
}

//constructeur par defaut
Mesh::Mesh()
{
        QString s=QDir::currentPath();
        std::string s2 = s.toUtf8().constData();
        offReader( s2+"/../queen.off");
        for(int i=0;i<1000000;i++)
        {
            color.push_back(Point((double)rand()/(double)RAND_MAX,(double)rand()/(double)RAND_MAX,(double)rand()/(double)RAND_MAX));
        }
}
// destructeur par defaut
Mesh::~Mesh()
{

}

// initialise un mesh2d
void Mesh::mesh2d()
{
    _vertices.clear();
    infinite_point.p1=Point(0,0,-1000);
    infinite_point.i1=-1;
    infinite_point.index=-1;
    _faces.clear();
    Vertice v(Point(1,0,0),1,0);
    _vertices.push_back(v);
    Vertice v1(Point(-1,0,0),0,1);
    _vertices.push_back(v1);
    Vertice v2(Point(0,1,0),0,2);
    _vertices.push_back(v2);
    Vertice v3(Point(0,-1,0),1,3);
    _vertices.push_back(v3);

    //0 1 0 2 4 5 1
    Face f1(1,0,2,4,5,1);
    _faces.push_back(f1);
    //1 0 1 3 2 3 0
    Face f2(0,1,3,2,3,0);
     _faces.push_back(f2);
    //2 -1 3 1 1 5 3
    Face f4(-1,3,1,1,5,3);
    _faces.push_back(f4);
    //3 -1 0 3 1 2 4
    Face f5(-1,0,3,1,2,4);
    _faces.push_back(f5);
    //4 -1 2 0 0 3 5
    Face f6(-1,2,0,0,3,5);
    _faces.push_back(f6);
    //5 -1 1 2 0 4 2
    Face f7(-1,1,2,0,4,2);
    _faces.push_back(f7);
}

//dessine un mesh
void Mesh::drawMesh()
{
    for(unsigned int i=0;i<_faces.size();i++)
    {
        glBegin(GL_TRIANGLES);
        glColor3d(color[i]._x,color[i]._y,color[i]._z);
        if(_faces[i].f1 !=-1 )
        {
            glPointDraw(_vertices[_faces[i].f1].p1);
        }
        else
            glPointDraw(infinite_point.p1);
        if(_faces[i].f2 !=-1)
            glPointDraw(_vertices[_faces[i].f2].p1);
        else {
            glPointDraw(infinite_point.p1);
        }
        if(_faces[i].f3 !=-1){
            glPointDraw(_vertices[_faces[i].f3].p1);
        }
        else {
            glPointDraw(infinite_point.p1);
        }
        glEnd();
    }
}

//affiche le mesh en wireframe
void Mesh::drawMeshWireFrame()
{
    for(unsigned int i=0;i<_faces.size();i++)
    {
        glColor3d((float)i/(_faces.size()-1),(float)(1-i/(_faces.size()-1)),(float)2*i/(_faces.size()-1));
        glBegin(GL_LINE_STRIP);
        glPointDraw(_vertices[_faces[i].f1].p1);
        glPointDraw(_vertices[_faces[i].f2].p1);
        glPointDraw(_vertices[_faces[i].f3].p1);
        glPointDraw(_vertices[_faces[i].f1].p1);
        glEnd();
    }
}


//CRUST

void Mesh::crust(std::string f)
{
    _vertices.clear();
    infinite_point.p1=Point(0,0,-1000);
    infinite_point.i1=-1;
    infinite_point.index=-1;
    _faces.clear();
    std::ifstream myfile;
    myfile.open(f);
    if (!myfile.is_open()) {
        qDebug()<<"je ne peux pas ouvrir " << f.c_str();
        exit(0);
    }
    int i,j,k;
    myfile >> i;
    myfile >> j;
    myfile >> k;

    for(int mi=0;mi<3;mi++)
    {
        float v1,v2;
        myfile >> v1;
        myfile >> v2;
        Vertice i1(Point(v1,v2,0),-1,mi);
        _vertices.push_back(i1);
    }
    Face f1=Face(0,1,2,1,2,3);
    _faces.push_back(f1);
    _faces.push_back(Face(2,1,-1,3,2,0));
    _faces.push_back(Face(0,2,-1,1,3,0));
    _faces.push_back(Face(1,0,-1,2,1,0));
    for(int mi=3;mi<i;mi++)
    {
        float v1,v2;
        myfile >> v1;
        myfile >> v2;
        delaunay_insertion(Point(v1,v2,0),1);
    }
    std::vector<Point> p=_Points_Voronoi;
    for(int mi=0;mi<p.size();mi++)
    {
        delaunay_insertion(p[mi],1);
    }
    for(int mi=0;mi<_faces.size();mi++)
    {
        int controle=0;
        Point pval1;
        Point pval2;
        Point p1=_vertices[_faces[mi].f1].p1;
        Point p2=_vertices[_faces[mi].f2].p1;
        Point p3=_vertices[_faces[mi].f3].p1;
        for(int mj=0;mj<p.size();mj++)
        {
            if(p1._x==p[mj]._x && p1._y==p[mj]._y && p1._z==p[mj]._z )
            {
                controle++;
                pval1=p2;
                pval2=p3;
            }
            else if(p3._x==p[mj]._x && p3._y==p[mj]._y && p3._z==p[mj]._z )
            {
                controle++;
                pval1=p1;
                pval2=p2;
            }
            else if(p2._x==p[mj]._x && p2._y==p[mj]._y && p2._z==p[mj]._z )
            {
                controle++;
                pval1=p1;
                pval2=p3;
            }
        }
        if(controle==1)
        {
            _Points_Crust.push_back(pval1);
            _Points_Crust.push_back(pval2);
        }
    }

}

//lecture un fichier

// lis un fichier .off et rempli la structure de données mesh
void Mesh::offReader(std::string s)
{
    std::ifstream myfile;
    myfile.open(s);
    if (!myfile.is_open()) {
        qDebug()<<"je ne peux pas ouvrir " << s.c_str();
        exit(0);
    }
    int i,j,k;
    myfile >> i;
    myfile >> j;
    myfile >> k;
    for(int v=0;v<i;v++)
    {
        float v1,v2,v3;
        myfile >> v1;
        myfile >> v2;
        myfile >> v3;
        Vertice i1(Point(v1,v2,v3),-1,v);
        _vertices.push_back(i1);
    }
    for(int f=0;f<j;f++)
    {
        int z;
        myfile >> z;
        if(z!=3)
            exit(1);
        int v1,v2,v3;
        myfile >> v1;
        myfile >> v2;
        myfile >> v3;
        Face f4(v1,v2,v3,-1,-1,-1);
        _faces.push_back(f4);
    }
    myfile.close();
    for(int f=0;f<j;f++)
    {
        std::pair<int,int> p1(_faces[f].f1,_faces[f].f2);
        std::pair<int,int> p2(_faces[f].f1,_faces[f].f3);
        std::pair<int,int> p3(_faces[f].f2,_faces[f].f3);
        std::pair<int,int> p4(_faces[f].f2,_faces[f].f1);
        std::pair<int,int> p5(_faces[f].f3,_faces[f].f1);
        std::pair<int,int> p6(_faces[f].f3,_faces[f].f2);
        setPair(p1,f);
        setPair(p2,f);
        setPair(p3,f);
        setPair(p4,f);
        setPair(p5,f);
        setPair(p6,f);
    }
    for(int f=0;f<j;f++)
    {
        if(_vertices[_faces[f].f1].i1==-1)
            _vertices[_faces[f].f1].i1=f;
        if(_vertices[_faces[f].f2].i1==-1)
            _vertices[_faces[f].f2].i1=f;
        if(_vertices[_faces[f].f3].i1==-1)
            _vertices[_faces[f].f3].i1=f;
    }
}

//crée/cherche une pair de vertex pour trouver la face voisine de la face f
void Mesh::setPair(std::pair<int,int> p,int f)
{
   _maps.insert({{p.first,p.second},f});
   int i=_maps[p];
   if(i!=f)
   {
        if(_faces[i].i1==-1)
        {
            _faces[i].i1=f;
        }
        else if(_faces[i].i2==-1 && _faces[i].i1 !=f)
        {
            _faces[i].i2=f;
        }
        else if(_faces[i].i3==-1 && _faces[i].i1 !=f && _faces[i].i2 !=f)
        {
            _faces[i].i3=f;
        }
        if(_faces[f].i1==-1)
        {
            _faces[f].i1=i;
        }
        else if(_faces[f].i2==-1 && _faces[f].i1 !=i)
        {
            _faces[f].i2=i;
        }
        else if(_faces[f].i3==-1 && _faces[f].i1 !=i && _faces[f].i2 !=i)
        {
            _faces[f].i3=i;
        }
    }
}

// iterator

//renvoie la face i
Face & Mesh::getFace(int i)
{
    return _faces[i];
}

//renvoie le vertex i
Vertice & Mesh::getVertex(int i)
{
    return _vertices[i];
}

//renvoie le premier Iterator on faces possible
Iterator_on_faces Mesh::faces_begin()
{
    Iterator_on_faces s;
    s._faces=&_faces;
    s.index=0;
    return s;
}

//renvoie le dernier Iterator on faces possible
Iterator_on_faces Mesh::faces_past_the_end()
{
    Iterator_on_faces s;
    s._faces=&_faces;
    s.index=_faces.size();
    return s;
}

//renvoie le premier Iterator on vertices possible
Iterator_on_vertices Mesh::vertice_begin()
{
    Iterator_on_vertices s;
    s._vertices=&_vertices;
    s.index=0;
    return s;
}

//renvoie le dernier Iterator on vertices possible
Iterator_on_vertices Mesh::vertice_past_the_end()
{
    Iterator_on_vertices s;
    s._vertices=&_vertices;
    s.index=_vertices.size();
    return s;
}

//initialise un circulator_on_faces
Circulator_on_faces Mesh::incident_faces(Vertice & v)
{
    Circulator_on_faces s;
    s._vertices=&_vertices;
    s._faces=&_faces;
    s.indexFaces=v.i1;
    s.indexVertices=v.index;
    return s;
}

//initialise un circulator_on_vertices
Circulator_on_vertices Mesh::adjacent_vertices(Vertice & v)
{
    Circulator_on_vertices s;
    s._vertices=&_vertices;
    s._faces=&_faces;
    s.indexFaces=v.i1;
    s.indexVertices=v.index;
    s.indiceVertex = _faces[s.indexFaces].f1;
    return s;

}

// utilitaire

//calcul le produit vectorielle d'un vecteur
Point cross(Point A,Point B)
{
    return Point( A._y * B._z - A._z * B._y ,
                  A._z * B._x - A._x * B._z ,
                  A._x * B._y - A._y * B._x );
}

// calcul le produit scalaire d'un vecteur
float dot(Point A, Point B)
{
    return  A._x * B._x + A._y * B._y + A._z * B._z;
}

// calcul la longueur d'un vecteur
float length(Point v)
{
    return std::sqrt(v._x * v._x + v._y * v._y + v._z * v._z);
}


//insertion


// verifie si un point est dans un triangle
float Mesh::in_triangle_face(int indexFace, Point p)
{
    Point A = _vertices[_faces[indexFace].f1].p1;
    Point B = _vertices[_faces[indexFace].f2].p1;
    Point C = _vertices[_faces[indexFace].f3].p1;

    float a = orientation_test(A,B,p);
    float b = orientation_test(B,C,p);
    float c = orientation_test(C,A,p);

    if(a>0 && b>0 && c>0)
        return 1;
    else if((a<=0 && a>= -0) || (b<=0 && b>= -0) || (c<=0 && c>= -0) )
        return 0;
    else
        return -1;
}

// cherche la face voisine de m qui soit i
int Mesh::searchFace(int i, int m)
{
    int f1=_faces[m].f1;
    int f2=_faces[m].f2;
    int f3= _faces[m].f3;
    if(f1==i)
        return _faces[m].i1;
    if(f2==i)
        return _faces[m].i2;
    if(f3==i)
        return _faces[m].i3;
    return 0;
}

// cherche la face voisine de m qui contient l'arrete i--j
int Mesh::searchFace2(int i,int j, int m)
{
    Face f1=_faces[_faces[m].i1];
    Face f2=_faces[_faces[m].i2];
    Face f3=_faces[_faces[m].i3];
    if(f1.contain(i,j) && _faces[m].i1 != m)
        return _faces[m].i1;
    if(f2.contain(i,j) && _faces[m].i2 != m)
        return _faces[m].i2;
    if(f3.contain(i,j) && _faces[m].i3 != m)
        return _faces[m].i3;
    return -1;
}

// copie de modifie point (les deux fonctions ont pas du être faites en même temps et oubliée)
void Mesh::modifieFace(int indexFace, int indexF1, int indexF2)
{
    if(_faces[indexFace].i1==indexF2)
        _faces[indexFace].i1=indexF1;
    else if(_faces[indexFace].i2==indexF2)
        _faces[indexFace].i2=indexF1;
    else if(_faces[indexFace].i3==indexF2)
        _faces[indexFace].i3=indexF1;

}


// coupe une face en trois faces/
void Mesh::split_face(int indexFace, Point p)
{
    int indiceF1= _faces[indexFace].f1;
    int indiceF2= _faces[indexFace].f2;
    int indiceF3= _faces[indexFace].f3;

    Vertice NEW = Vertice(p,indexFace,_vertices.size());
    _vertices.push_back(NEW);

    std::pair<int,int> p1(indiceF1,indiceF2);
    std::pair<int,int> p2(indiceF2,indiceF3);
    std::pair<int,int> p3(indiceF3,indiceF1);

    int indiceFace1 = searchFace(indiceF3,indexFace);
    int indiceFace2 = searchFace(indiceF1,indexFace);
    Face f1 = Face(indiceF1,indiceF2,NEW.index,
                   _faces.size()+1,indexFace,indiceFace1);
    Face f2 = Face(indiceF2,indiceF3,NEW.index,
                   indexFace,_faces.size(),indiceFace2);

    _faces[indexFace].f2 = NEW.index;
    modifieFace(indexFace,_faces.size()+1,indiceFace2);
    modifieFace(indexFace,_faces.size(),indiceFace1);
    modifieFace(indiceFace1,_faces.size(),indexFace);
    modifieFace(indiceFace2,_faces.size()+1,indexFace);

    std::pair<int,std::pair<int,int>> pr1(indexFace,p3);
    std::pair<int,std::pair<int,int>> pr2(_faces.size(),p1);
    std::pair<int,std::pair<int,int>> pr3(_faces.size()+1,p2);

    arretes_to_flip.push(pr1);
    arretes_to_flip.push(pr2);
    arretes_to_flip.push(pr3);

    _faces.push_back(f1);
    _faces.push_back(f2);
}

// remplace la valeur indiceface de la face indicefacecut par indiceface2
void Mesh::modifiePoint(int indicefacecut, int indiceface,int indiceface2)
{
    Face f3 = _faces[indicefacecut];
    if(f3.i1 == indiceface)
        _faces[indicefacecut].i1=indiceface2;
    if(f3.i2 == indiceface)
        _faces[indicefacecut].i2=indiceface2;
    if(f3.i3 == indiceface)
        _faces[indicefacecut].i3=indiceface2;
}

// delaunay

// cherche le point de la face qui ne compose pas l'arrête indice1---indice2
int chercheNonSommet(Face f2, int indice1, int indice2)
{
    if((f2.f1==indice1 && f2.f2==indice2) || (f2.f2==indice1 && f2.f1==indice2))
    {
        return f2.f3;
    }
    if((f2.f3==indice1 && f2.f1==indice2) || (f2.f1==indice1 && f2.f3==indice2))
    {
        return f2.f1;
    }
    if((f2.f2==indice1 && f2.f3==indice2) || (f2.f3==indice1 && f2.f2==indice2) )
    {
        return f2.f2;
    }
    return -1;
}


// Cherche les valeurs a flip en fonction de la face
void Mesh::ChercheValeurFLip(int tab[6], Face f1, int indexF1,int indexF2)
{
    tab[0] = f1.f1;
    tab[1]  = f1.f2;
    tab[2]  = f1.f3;
    tab[3]  = f1.i1;
    tab[4]  = f1.i2;
    tab[5]  = f1.i3;
    if(searchFace2(tab[0],tab[1] ,indexF1) != indexF2)
    {
        tab[0] = f1.f2;
        tab[1] = f1.f3;
        tab[2] = f1.f1;
        tab[3] = f1.i2;
        tab[4] = f1.i3;
        tab[5] = f1.i1;
        if(searchFace2(tab[0] ,tab[1] ,indexF1) != indexF2)
        {
            tab[0] = f1.f3;
            tab[1] = f1.f1;
            tab[2] = f1.f2;
            tab[3] = f1.i3;
            tab[4] = f1.i1;
            tab[5] = f1.i2;
        }
    }
}


// flip une arrête entre deux faces
void Mesh::edge_flip(int indexF1, int indexF2)
{
    Face f1 =_faces[indexF1];
    Face f2 =_faces[indexF2];

    int tabIndice1[6];
    int tabIndice2[6];

    ChercheValeurFLip(tabIndice1, f1, indexF1,indexF2);
    ChercheValeurFLip(tabIndice2, f2, indexF2,indexF1);


    int indiceFace2Cut1,indiceFace2Cut2, indiceFace2Cut3;
    indiceFace2Cut3=searchFace2(tabIndice2[1],tabIndice2[2],indexF2);
    indiceFace2Cut1=searchFace2(tabIndice1[1],tabIndice1[2],indexF1);
    modifiePoint(indiceFace2Cut1, indexF1,indexF2);
    modifiePoint(indiceFace2Cut3, indexF2,indexF1);

    _faces[indexF1].f1=tabIndice1[2];
    _faces[indexF1].i1=tabIndice2[3];
    _faces[indexF1].f2=tabIndice1[0];
    _faces[indexF1].i2=tabIndice1[5];
    _faces[indexF1].f3=tabIndice2[2];
    _faces[indexF1].i3=tabIndice1[4];

    _faces[indexF2].f1=tabIndice2[2];
    _faces[indexF2].i1=tabIndice1[3];
    _faces[indexF2].f2=tabIndice2[0];
    _faces[indexF2].i2=tabIndice2[5];
    _faces[indexF2].f3=tabIndice1[2];
    _faces[indexF2].i3=tabIndice2[4];
}

//vérifie si une arrête est visible par le point
double Mesh::orientation_test(Point pointA, Point pointB, Point pointC)
{
    Point AB=Point(pointB._x-pointA._x,pointB._y-pointA._y,pointB._z-pointA._z);
    Point AC=Point(pointC._x-pointA._x,pointC._y-pointA._y,pointC._z-pointA._z);
    Point k(0,0,1);
    double x=dot(cross(AB,AC),k);
    return x;
}


// si on insère en dehors du mesh flip deux faces infinis pour garder le mesh convexe.
int Mesh::flip_face_point(int tabindice1[2], int & v, Point point, int & vprec)
{
    int controle=0;
    int i=searchFace2(tabindice1[0],tabindice1[1],v);

    if(_faces[i].f1==-1)
    {
        if(orientation_test(_vertices[_faces[i].f2].p1,_vertices[_faces[i].f3].p1,point)>0)
        {
            edge_flip(v, i);
            if(_faces[i].f1==-1 || _faces[i].f2==-1 || _faces[i].f3==-1)
                v=i;
            else
                vprec=i;
            controle=1;
        }
    }
    else if(_faces[i].f2==-1)
    {
        if(orientation_test(_vertices[_faces[i].f3].p1,_vertices[_faces[i].f1].p1,point)>0)
        {
            edge_flip(v, i);
            if(_faces[i].f1==-1 || _faces[i].f2==-1 || _faces[i].f3==-1)
                v=i;
            else
                vprec=i;
            controle=1;
        }
    }
    else if(_faces[i].f3==-1)
    {
        if(orientation_test(_vertices[_faces[i].f1].p1,_vertices[_faces[i].f2].p1,point)>0)
        {
            edge_flip(v, i);
            if(_faces[i].f1==-1 || _faces[i].f2==-1 || _faces[i].f3==-1)
                v=i;
            else
                vprec=i;
            controle=1;
        }
    }
    return controle;
}


// si on fait une insertion en dehors du point infini split une face infini visible par le point
int Mesh::split_infini(int i, Point point)
{
    int controle=-1;
    if(_faces[i].f1==-1)
    {
        if(orientation_test(_vertices[_faces[i].f2].p1,_vertices[_faces[i].f3].p1,point)>0)
        {
            split_face(i, point);
            controle=i;
        }
    }
    else if(_faces[i].f2==-1)
    {
        if(orientation_test(_vertices[_faces[i].f3].p1,_vertices[_faces[i].f1].p1,point)>0)
        {
            split_face(i, point);
            controle=i;
        }
    }
    else if(_faces[i].f3==-1)
    {
        if(orientation_test(_vertices[_faces[i].f1].p1,_vertices[_faces[i].f2].p1,point)>0)
        {
            split_face(i, point);
            controle=i;
        }
    }
    return controle;
}

// si on insère en dehors du mesh, récupère l'arrête séparant deux points infinis après l'insertion
void get_arretes_sep(Face f, int tab[2], int point)
{
    if(f.f1==point)
    {
        tab[0]=f.f2;
        tab[1]=f.f3;
    }
    if(f.f2==point)
    {
        tab[0]=f.f3;
        tab[1]=f.f1;
    }
    if(f.f3==point)
    {
        tab[0]=f.f1;
        tab[1]=f.f2;
    }
}


// si on fait une insertion hors du mesh ajoutes toutes les arrêtes dont on doit vérifier si elle vérifie delaunay ou non
void Mesh::set_edges_flip(int v, int tabindice1)
{
    std::pair<int,int> p1;
    std::pair<int,int> p2;

    if(_faces[v].f1==tabindice1)
    {
        p1=std::pair<int,int>(tabindice1,_faces[v].f2);
        p2=std::pair<int,int>(tabindice1,_faces[v].f3);
    }
    if(_faces[v].f2==tabindice1)
    {
        p1=std::pair<int,int>(tabindice1,_faces[v].f1);
        p2=std::pair<int,int>(tabindice1,_faces[v].f3);
    }
    if(_faces[v].f3==tabindice1)
    {
        p1=std::pair<int,int>(tabindice1,_faces[v].f2);
        p2=std::pair<int,int>(tabindice1,_faces[v].f1);
    }
    std::pair<int,std::pair<int,int>> pr1(v,p2);
    std::pair<int,std::pair<int,int>> pr2(v,p1);
    arretes_to_flip.push(pr1);
    arretes_to_flip.push(pr2);
}


// si on insère un point avec une face infini connect le point à toutes les faces visible pour garder un mesh convexe
void Mesh::rempli_faces(int v,int indiceV,Point point)
{
    bool controle=1;
    while (controle)
    {
        Face f1=_faces[v];
        int vprec=v;
        int tabindice1[2];
        get_arretes_sep(f1, tabindice1, indiceV);
        controle=flip_face_point(tabindice1,v,point,vprec);
        if(controle==1)
        {
            if(tabindice1[0] == -1)
            {
                set_edges_flip(vprec,tabindice1[1]);
            }
            else {
                set_edges_flip(vprec,tabindice1[0]);
            }
        }
    }
}


// insère un point naivement
void Mesh::naive_insertion(Point point)
{
    int in_triangle=-1;
    for(unsigned int i=0; i <_faces.size();i++)
    {
        float verif=in_triangle_face(i, point);
        if(verif>0)
        {
            in_triangle=i;
            break;
        }
    }
    if(in_triangle != -1)
    {
        split_face(in_triangle, point);
    }
    else {
        int v=-1;
        for(unsigned int i=0; i <_faces.size();i++)
        {
            v = split_infini(i,point);
            if (v!=-1)
            {
                v=i;
                break;
            }
        }
        if (v==-1)
        {
            return;
        }
        if(_faces[v].f1==-1 || _faces[v].f2==-1 || _faces[v].f3==-1)
            rempli_faces(v,_vertices.size()-1,point);
        v=_faces.size()-1;
        if(_faces[v].f1==-1 || _faces[v].f2==-1 || _faces[v].f3==-1)
            rempli_faces(v,_vertices.size()-1,point);
        v=_faces.size()-2;
        if(_faces[v].f1==-1 || _faces[v].f2==-1 || _faces[v].f3==-1)
            rempli_faces(v,_vertices.size()-1,point);
    }
}

// insère un point selon l'algorithme de marche
void Mesh::marcheInsertion(Point point)
{
    int f1=rand()%(_faces.size()-1);
    Face f=_faces[f1];
    while(f.f1==-1 || f.f2==-1 || f.f3==-1)
    {
        f1=rand()%(_faces.size()-1);
        f=_faces[f1];
    }
    bool continues=1;
    while(continues)
    {
        if(in_triangle_face(f1, point)>0)
        {
            continues=0;
            split_face(f1, point);
        }
        else if (f.f1==-1 || f.f2==-1 || f.f3==-1 ){
            int v = split_infini(f1,point);
            if (v==-1)
            {
                return;
            }
            if(_faces[v].f1==-1 || _faces[v].f2==-1 || _faces[v].f3==-1)
                rempli_faces(v,_vertices.size()-1,point);
            v=_faces.size()-1;
            if(_faces[v].f1==-1 || _faces[v].f2==-1 || _faces[v].f3==-1)
                rempli_faces(v,_vertices.size()-1,point);
            v=_faces.size()-2;
            if(_faces[v].f1==-1 || _faces[v].f2==-1 || _faces[v].f3==-1)
                rempli_faces(v,_vertices.size()-1,point);
            continues=0;
        }
        else {
            if(orientation_test(_vertices[f.f1].p1,_vertices[f.f2].p1,point)<-0)
            {
                int f2=searchFace2(f.f1,f.f2,f1);
                f1=f2;
                f=_faces[f1];
            }
            else if(orientation_test(_vertices[f.f2].p1,_vertices[f.f3].p1,point)<-0)
            {
                int f2=searchFace2(f.f2,f.f3,f1);
                f1=f2;
                f=_faces[f1];
            }
            else if(orientation_test(_vertices[f.f3].p1,_vertices[f.f1].p1,point)<-0)
            {
                int f2=searchFace2(f.f3,f.f1,f1);
                f1=f2;
                f=_faces[f1];
            }
            else {
                continues=0;
            }

        }

    }
}

// récupère le tableau des points de l'image
std::vector<Vertice> Mesh::getVertice()
{
    return _vertices;
}


//vérifie si un triangle est de delaunay
double Mesh::verif_point_delau(Point s, Face f)
{
    Point q,p,r;
    if(f.f2!=-1)
        q = _vertices[f.f2].p1;
    else
        q = infinite_point.p1;
    if(f.f1!=-1)
        p = _vertices[f.f1].p1;
    else
        p = infinite_point.p1;
    if(f.f3!=-1)
        r = _vertices[f.f3].p1;
    else
        r = infinite_point.p1;

    Point phiq(q._x,q._y,q._x*q._x+q._y*q._y);
    Point phip(p._x,p._y,p._x*p._x+p._y*p._y);
    Point phir(r._x,r._y,r._x*r._x+r._y*r._y);
    Point phis(s._x,s._y,s._x*s._x+s._y*s._y);

    Point r1(phiq._x-phip._x,phiq._y-phip._y,phiq._z-phip._z);
    Point r2(phir._x-phip._x,phir._y-phip._y,phir._z-phip._z);
    Point r3(phis._x-phip._x,phis._y-phip._y,phis._z-phip._z);

    return -1.0*dot(cross(r1,r2),r3);
}



//insère les points et vérifie si le mesh est de delaunay, si ce n'est pas le cas le transforme
void Mesh::delaunay_insertion(Point point, int mode)
{
    if (mode==0)
        naive_insertion(point);
    if (mode==1)
        marcheInsertion(point);
    int pointV=_vertices.size()-1;
    while(!(arretes_to_flip.empty()))
    {
        int indice1 =((arretes_to_flip.front()).second).first;
        int indice2 =((arretes_to_flip.front()).second).second;
        int face2 = (arretes_to_flip.front()).first;
        arretes_to_flip.pop();
        int face1 = searchFace2(indice1,indice2,face2);
        int indicePoint = -1;
        if(face1!=-1)
        {
            if((_faces[face1].f1==indice1 && _faces[face1].f2==indice2) ||  (_faces[face1].f2==indice1 && _faces[face1].f1==indice2))
                indicePoint=_faces[face1].f3;
            if((_faces[face1].f2==indice1 && _faces[face1].f3==indice2) ||  (_faces[face1].f3==indice1 && _faces[face1].f2==indice2))
                indicePoint=_faces[face1].f1;
            if((_faces[face1].f1==indice1 && _faces[face1].f3==indice2) ||  (_faces[face1].f3==indice1 && _faces[face1].f1==indice2))
                indicePoint=_faces[face1].f2;
        }
        double result = -1;
        if(indice1 !=-1 && indice2 != -1 && indicePoint !=-1)
        {
            result=verif_point_delau(_vertices[indicePoint].p1, _faces[face2]);
        }
        if(result>0)
        {
            edge_flip(face2, face1);
            std::pair<int,int> p1(indicePoint,indice1);
            std::pair<int,int> p2(indicePoint,indice2);
            std::pair<int,std::pair<int,int>> pr1,pr2;
            if(_faces[face1].f1==indice1 || _faces[face1].f2==indice1 || _faces[face1].f3==indice1)
            {
                pr1= std::pair<int,std::pair<int,int>>(face1,p1);
                pr2= std::pair<int,std::pair<int,int>>(face2,p2);
            }
            else {
                pr1= std::pair<int,std::pair<int,int>>(face1,p2);
                pr2= std::pair<int,std::pair<int,int>>(face2,p1);
            }
            arretes_to_flip.push(pr1);
            arretes_to_flip.push(pr2);
        }
    }
    remakeVertex();
    voronoi();
}


void Mesh::remakeVertex()
{
    for(int i=0;i<_faces.size();i++)
    {
        for(int j=0;j<_vertices.size();j++)
        {
            if(j==_faces[i].f1)
            {
                _vertices[j].i1=i;
            }
            if(j==_faces[i].f2)
            {
                _vertices[j].i1=i;
            }
            if(j==_faces[i].f3)
            {
                _vertices[j].i1=i;
            }
        }
    }
}

// voronoi

//calcul le centre du cercle circonscrit pour une face du mesh
Point * Mesh::makeCircumcircle(const Point &a, const Point &b, const Point &c) {
    double ox = (std::min(std::min(a._x, b._x), c._x) + std::max(std::min(a._x, b._x), c._x)) / 2;
    double oy = (std::min(std::min(a._y, b._y), c._y) + std::max(std::min(a._y, b._y), c._y)) / 2;
    double ax = a._x - ox,  ay = a._y - oy;
    double bx = b._x - ox,  by = b._y - oy;
    double cx = c._x - ox,  cy = c._y - oy;
    double d = (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by)) * 2;
    if (d == 0)
        return nullptr;
    double x = ((ax*ax + ay*ay) * (by - cy) + (bx*bx + by*by) * (cy - ay) + (cx*cx + cy*cy) * (ay - by)) / d;
    double y = ((ax*ax + ay*ay) * (cx - bx) + (bx*bx + by*by) * (ax - cx) + (cx*cx + cy*cy) * (bx - ax)) / d;
    Point * p=new Point(ox + x, oy + y,0);
    return p;
}

//crée l'ensemble des points de voronoi du diagramme
void Mesh::voronoi()
{
    _Points_Voronoi.clear();
    for(int i=0;i<_faces.size();i++)
    {
        Face f=_faces[i];
        if(f.f1!=-1 && f.f2 != -1 && f.f2 != -1)
        {
            Point * p=makeCircumcircle(_vertices[f.f1].p1,_vertices[f.f2].p1,_vertices[f.f3].p1);
            if(p!=nullptr)
                _Points_Voronoi.push_back(*p);
        }
        else {
            if(f.f1==-1)
            {
                Point p1=_vertices[f.f2].p1;
                Point p2=_vertices[f.f3].p1;
                Point vect(p2._x-p1._x,p2._y-p1._y,p2._z-p1._z);
                float dist=length(vect);
                Point vectn(vect._x/dist,vect._y/dist,vect._z/dist);
                Point normale(-vect._y,vect._x,0);
                Point demivect(vect._x/2,vect._y/2,vect._z/2);
                Point milieu(p1._x + demivect._x,p1._y + demivect._y,p1._z + demivect._z);
                Point resultat(milieu._x+1000*normale._x,milieu._y+1000*normale._y,milieu._z+1000*normale._z);
                _Points_Voronoi.push_back(resultat);
            }
            if(f.f2==-1)
            {
                Point p1=_vertices[f.f3].p1;
                Point p2=_vertices[f.f1].p1;
                Point vect(p2._x-p1._x,p2._y-p1._y,p2._z-p1._z);
                float dist=length(vect);
                Point vectn(vect._x/dist,vect._y/dist,vect._z/dist);
                Point normale(-vect._y,vect._x,0);
                Point demivect(vect._x/2,vect._y/2,vect._z/2);
                Point milieu(p1._x + demivect._x,p1._y + demivect._y,p1._z + demivect._z);
                Point resultat(milieu._x+1000*normale._x,milieu._y+1000*normale._y,milieu._z+1000*normale._z);
                _Points_Voronoi.push_back(resultat);
            }
            if(f.f3==-1)
            {
                Point p1=_vertices[f.f1].p1;
                Point p2=_vertices[f.f2].p1;
                Point vect(p2._x-p1._x,p2._y-p1._y,p2._z-p1._z);
                float dist=length(vect);
                Point vectn(vect._x/dist,vect._y/dist,vect._z/dist);
                Point normale(-vect._y,vect._x,0);
                Point demivect(vect._x/2,vect._y/2,vect._z/2);
                Point milieu(p1._x + demivect._x,p1._y + demivect._y,p1._z + demivect._z);
                Point resultat(milieu._x+1000*normale._x,milieu._y+1000*normale._y,milieu._z+1000*normale._z);
                _Points_Voronoi.push_back(resultat);
            }
        }
    }

}
