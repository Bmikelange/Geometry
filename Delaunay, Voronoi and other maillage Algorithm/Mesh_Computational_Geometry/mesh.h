#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include "iterator_on_faces.h"
#include "iterator_on_vertices.h"
#include "utilitaire.h"
#include "circulator_on_faces.h"
#include "circulator_on_vertices.h"
#include "queue"

// TO MODIFY


class Mesh
{
  std::vector<Vertice> _vertices;
  std::vector<Face> _faces;
  Vertice infinite_point;
  std::map<std::pair<int,int>,int> _maps;
  std::queue<std::pair<int,std::pair<int,int>>> arretes_to_flip;

public:
    Mesh(); // Constructors automatically called to initialize a Mesh (default strategy)
    ~Mesh(); // Destructor automatically called before a Mesh is destroyed (default strategy)
    void drawMesh();
    void drawMeshWireFrame();
    void offReader(std::string s);
    void setPair(std::pair<int,int> p,int f);
    Face & getFace(int i);
    Vertice & getVertex(int i);
    Iterator_on_faces faces_begin();
    Iterator_on_faces faces_past_the_end();
    Iterator_on_vertices vertice_begin();
    Iterator_on_vertices vertice_past_the_end();
    Circulator_on_faces incident_faces(Vertice & v);
    Circulator_on_vertices adjacent_vertices(Vertice & v);
    std::vector<Vertice> getVertice();
    void split_face(int indexFace,Point p);
    float in_triangle_face(int indexFace,Point p);
    void edge_flip(int indexF1, int indexF2);
    double orientation_test(Point pointA,Point pointB,Point pointC);
    void naive_insertion(Point p);
    int searchFace(int i, int m);
    int searchFace2(int i, int j, int m);
    void modifieFace(int indexFace, int indexF1, int indexF2);
    void mesh2d();
    void modifiePoint(int indicefacecut, int indiceface,int indiceface2);
    void delaunay_insertion(Point point,int mode);
    double verif_point_delau(Point s, Face f);
    void flip_delaunay(int i, int j);
    int split_infini(int i,Point point);
    int flip_face_point(int tabindice[2], int & v,Point point,int & vprec);
    void ChercheValeurFLip(int tab[6], Face f1, int indexF1,int indexF2);
    void rempli_faces(int v,int indiceV,Point point);
    void set_edges_flip(int v,int tabindice1);
    void marcheInsertion(Point point);
    void remakeVertex();
    void crust(std::string f);

    std::vector<Point> color;
    std::vector<Point> _Points_Voronoi;
    std::vector<Point> _Points_Crust;

    Point * makeCircumcircle(const Point &a, const Point &b, const Point &c);
    void voronoi();
};

class GeometricWorld //Generally used to create a singleton instance
{
  QVector<Point> _bBox;  // Bounding box
public :
  GeometricWorld();
  void draw();
  void drawWireFrame();
  // ** TP Can be extended with further elements;
  // Mesh _mesh;
};


#endif // MESH_H
