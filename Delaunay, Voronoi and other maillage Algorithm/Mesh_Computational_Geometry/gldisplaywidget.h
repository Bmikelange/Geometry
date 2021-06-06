#ifndef GLDISPLAYWIDGET_H
#define GLDISPLAYWIDGET_H

#include <QGLWidget> // Module QtOpengL (classes QGLxxx in Qt4),
                     // with widget and rendering classes descending from QGLWidget,
                     // deprecated for module QtGui (classes QOpenGLxxx )
                     // and module widgets (QWidget, QGraphicsView).
#include <QtWidgets>
#include <QTimer>
#include "mesh.h"  // Model
#include "iterator_on_faces.h"
#include "iterator_on_vertices.h"
#include "circulator_on_faces.h"
#include "circulator_on_vertices.h"
#include "laplacian.h"

class GLDisplayWidget : public QGLWidget
{
public:
    explicit GLDisplayWidget(QWidget *parent = 0);

    void initializeGL(); // The scene may be initialized in this function since the GeometricWorld is a data member...
    void paintGL(); // Display the scene Gl
    void resizeGL(int width, int height);
    void drawIteraror();
    void drawIterarorV();
    void drawCirculator();
    void drawCirculatorV();
    void drawVoronois();
    void drawCrusts();

public slots:
    void changeAffichage(int arg1);
    void laplacians(int arg1);
    void incrementeIterator();
    void incrementeIteratorV();
    void incrementeCirculator();
    void incrementeCirculatorV();
    void invertAxis();
    void marche();
    void switchto2d(int arg1);
    void drawVoronoi();
    void switchtoCrust(int arg1);
    void drawCrust();

protected:
    // Mouse Management
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:

    QTimer _timer; // To update the scene
    float _X, _Y ,_Z; // Translation
    float _angle; // Rotation

    QPoint _lastPosMouse; // To keep the last position of the mouse

    GeometricWorld _geomWorld; // The scene to be displayed
    Mesh m;
    Mesh mesh2d;
    Mesh meshCrust;

   Iterator_on_faces itonf;
   Iterator_on_vertices itonv;
   Circulator_on_faces cironf;
   Circulator_on_vertices cironv;
   laplacian lap;


    QCheckBox * check;
    QPushButton * button;
    QPushButton * buttonV;
    QPushButton * buttonC;
    QPushButton * buttonCV;
    QPushButton * buttonInvertAxis;
    QPushButton * buttonMarche;
    QPushButton * buttonVoronoi;
    QCheckBox * checkLap;
    QCheckBox * check2d;
    QCheckBox * checkCrust;
    QPushButton * buttonDrawCrust;
    int controle=0;
    int controlelap=0;
    int controle2d=0;
    int voronoi=0;
    int controleCrust=0;
    int controleCrust2=0;

    std::mt19937 generator;
    std::uniform_real_distribution<double> distribution;
};

#endif // GLDISPLAYWIDGET_H
