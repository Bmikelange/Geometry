#include "gldisplaywidget.h"
#ifdef __APPLE__
    #include <glu.h>
#else
    #include <GL/glu.h>
#endif

#include "QDebug"
#include <random>


GLDisplayWidget::GLDisplayWidget(QWidget *parent) : QGLWidget(parent),_X(0),_Y(0),_Z(0)
{
    check=new QCheckBox("wireframe",this);
    check->setStyleSheet("QCheckBox {background-color:black; color: red }");
    check->move(600,400);
    checkCrust=new QCheckBox("Crust",this);
    checkCrust->setStyleSheet("QCheckBox {background-color:black; color: red }");
    checkCrust->move(600,310);
    mesh2d.mesh2d();
    meshCrust.crust("../FichierTest5.xy");
    //Iterator
    itonf=m.faces_begin();
    itonv=m.vertice_begin();
    cironf=m.incident_faces(m.getVertex(5));
    cironv=m.adjacent_vertices(m.getVertex(5));
    lap.mesh=m;
    lap.itonv=m.vertice_begin();
    lap.computeLaplacian();
    button= new QPushButton("visit face",this);
    button->move(600,370);
    button->setFixedSize(0,0);
    buttonDrawCrust= new QPushButton("final",this);
    buttonDrawCrust->move(600,370);
    buttonDrawCrust->setFixedSize(0,0);
    buttonV= new QPushButton("visit vertex",this);
    buttonV->move(600,340);
    buttonV->setFixedSize(0,0);
    buttonC= new QPushButton("circule face",this);
    buttonC->move(600,310);
    buttonC->setFixedSize(0,0);
    buttonCV= new QPushButton("circule vertices",this);
    buttonCV->move(600,280);
    buttonCV->setFixedSize(0,0);
    buttonInvertAxis= new QPushButton("point delaunay",this);
    buttonInvertAxis->move(600,370);
    buttonInvertAxis->setFixedSize(0,0);
    buttonVoronoi= new QPushButton("voronoi",this);
    buttonVoronoi->move(470,400);
    buttonVoronoi->setFixedSize(0,0);
    buttonMarche= new QPushButton("point marche",this);
    buttonMarche->move(600,400);
    buttonMarche->setFixedSize(0,0);
    checkLap=new QCheckBox("laplacian",this);
    checkLap->setStyleSheet("QCheckBox {background-color:black; color: red }");
    checkLap->move(600,370);
    check2d=new QCheckBox("2D",this);
    check2d->setStyleSheet("QCheckBox { background-color:black;color: red }");
    check2d->move(600,340);
    checkLap->setFixedSize(100,25);
    check2d->setFixedSize(100,25);
    check->setFixedSize(100,25);
    checkCrust->setFixedSize(100,25);
    // Update the scene
    connect( &_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    connect(check, &QCheckBox::stateChanged, this, &GLDisplayWidget::changeAffichage);
    connect(button, &QPushButton::clicked, this, &GLDisplayWidget::incrementeIterator);
    connect(buttonV, &QPushButton::clicked, this, &GLDisplayWidget::incrementeIteratorV);
    connect(buttonC, &QPushButton::clicked, this, &GLDisplayWidget::incrementeCirculator);
    connect(buttonCV, &QPushButton::clicked, this, &GLDisplayWidget::incrementeCirculatorV);
    connect(buttonInvertAxis, &QPushButton::clicked, this, &GLDisplayWidget::invertAxis);
    connect(buttonMarche, &QPushButton::clicked, this, &GLDisplayWidget::marche);
    connect(buttonVoronoi, &QPushButton::clicked, this, &GLDisplayWidget::drawVoronoi);
    connect(buttonDrawCrust, &QPushButton::clicked, this, &GLDisplayWidget::drawCrust);
    connect(checkLap, &QCheckBox::clicked, this, &GLDisplayWidget::laplacians);
    connect(check2d, &QCheckBox::clicked, this, &GLDisplayWidget::switchto2d);
    connect(checkCrust, &QCheckBox::clicked, this, &GLDisplayWidget::switchtoCrust);
    _timer.start(16); // Starts or restarts the timer with a timeout interval of 16 milliseconds.


    //init random
    std::random_device device;
    generator=std::mt19937(device());
    distribution=std::uniform_real_distribution<double>(-1.0,1.0);



}

// Draw a Point
void glPointDraw2(const Point & p) {
    glVertex3f(p._x, p._y, p._z);
}


// affiche un point dans le diagramme de voronoi
void verif(Circulator_on_faces & cironv2d, Mesh & mesh2d)
{
    Point p;
    Face f=mesh2d.getFace(cironv2d.indexFaces);
    p=mesh2d._Points_Voronoi[cironv2d.indexFaces];
    p._z=0.01;
    glPointDraw2(p);
}

void GLDisplayWidget::drawCrusts()
{
    glBegin(GL_LINES);
    for(int i=0;i<meshCrust._Points_Crust.size()-1;i+=2)
    {
        glPointDraw2(meshCrust._Points_Crust[i]);
        glPointDraw2(meshCrust._Points_Crust[i+1]);
    }
    glEnd();
}

void GLDisplayWidget::drawCrust()
{
    if(controleCrust2==0)
        controleCrust2=1;
    else {
        controleCrust2=0;
    }
}


// dessine le diagram de voronoi
void GLDisplayWidget::drawVoronois()
{
    if(controleCrust==0)
    {
        std::vector<Vertice> v=mesh2d.getVertice();
        if(mesh2d._Points_Voronoi.size()==0)
            return;
        for(int i=0;i<v.size();i++)
        {
            Circulator_on_faces cironv2d=mesh2d.incident_faces(v[i]);
            Circulator_on_faces cironv2df=mesh2d.incident_faces(v[i]);
            glColor3d(0,0,1);
            glBegin(GL_LINE_STRIP);
            verif(cironv2d, mesh2d);
            cironv2d++;
            while(cironv2d != cironv2df)
            {
                verif(cironv2d, mesh2d);
                cironv2d++;
            }
            glEnd();
        }
    }
    else {
        std::vector<Vertice> v=meshCrust.getVertice();
        if(meshCrust._Points_Voronoi.size()==0)
            return;
        for(int i=0;i<v.size();i++)
        {
            Circulator_on_faces cironv2d=meshCrust.incident_faces(v[i]);
            Circulator_on_faces cironv2df=meshCrust.incident_faces(v[i]);
            glColor3d(0,0,1);
            glBegin(GL_LINE_STRIP);
            verif(cironv2d, meshCrust);
            cironv2d++;
            while(cironv2d != cironv2df)
            {
                verif(cironv2d, meshCrust);
                cironv2d++;
            }
            glEnd();
        }
    }
}

// affiche ou cache le diagramme de voronoi
void GLDisplayWidget::drawVoronoi()
{
    if(voronoi==0)
        voronoi=1;
    else {
        voronoi=0;
    }
}

void GLDisplayWidget::initializeGL()
{
    // background color
    glClearColor(0.2, 0.2, 0.2, 1);

    // Shader
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    //** TP : To add....
    // Construction of the GeometricWorld before it is displayed
    // It can also be constructed following a signal (button)
}

void GLDisplayWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Center the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,  0,0,0,   0,1,0); //gluLookAt(eye, center, up)  //deprecated
                                       // Returns a 4x4 matrix that transforms world coordinates to eye coordinates.
    // Translation
    glTranslated(_X, _Y, _Z);

    // Rotation
    glRotatef(_angle, 1.0f, 1.0f, 0.0f);

    // Color for your _geomWorld
    glColor3f(0, 1 ,0);

    // example with a tetraedre
    _geomWorld.drawWireFrame();
    if(controleCrust==0)
    {
        if(controle2d==0)
        {
            if(controlelap==0)
            {
                if(controle==2)
                {
                    m.drawMeshWireFrame();
                    drawIteraror();
                    drawIterarorV();
                    drawCirculator();
                    drawCirculatorV();
                }
                else {
                    m.drawMesh();
                }
            }
            else {
                lap.drawMeshlap();
            }
        }
        else {
            mesh2d.drawMesh();
            if(voronoi==1)
            {
                drawVoronois();
            }
        }
     }
     else {
        if(controleCrust2==0)
        {
             meshCrust.drawMesh();
             if(voronoi==1)
             {
                 drawVoronois();
             }
        }else {
            drawCrusts();
        }
    }

    //_geomWorld.draw();
}


//affiche le mesh 3d en mode wirframe
void GLDisplayWidget::changeAffichage(int arg1)
{
    controle=arg1;
    if(arg1==0)
    {
        button->setFixedSize(0,0);
        buttonC->setFixedSize(0,0);
        buttonV->setFixedSize(0,0);
        buttonCV->setFixedSize(0,0);
        checkLap->setFixedSize(100,25);
        check2d->setFixedSize(100,25);
        checkCrust->setFixedSize(100,25);
    }
    else {
        button->setFixedSize(100,25);
        buttonC->setFixedSize(100,25);
        buttonV->setFixedSize(100,25);
        buttonCV->setFixedSize(100,25);
        checkLap->setFixedSize(0,0);
        check2d->setFixedSize(0,0);
        checkCrust->setFixedSize(100,25);
    }
}


//affiche le mesh 2d
void GLDisplayWidget::switchto2d(int arg1)
{
    controle2d=arg1;
    if(arg1==0)
    {
        checkCrust->setFixedSize(100,25);
        check->setFixedSize(100,25);
        checkLap->setFixedSize(100,25);
        buttonInvertAxis->setFixedSize(0,0);
        buttonVoronoi->setFixedSize(0,0);
        buttonMarche->setFixedSize(0,0);
    }
    else {
        checkCrust->setFixedSize(0,0);
        check->setFixedSize(0,0);
        checkLap->setFixedSize(0,0);
        buttonInvertAxis->setFixedSize(100,25);
        buttonMarche->setFixedSize(100,25);
        buttonVoronoi->setFixedSize(100,25);

    }
}

void GLDisplayWidget::switchtoCrust(int arg1)
{
    controleCrust=arg1;
    if(arg1==0)
    {
        check2d->setFixedSize(100,25);
        check->setFixedSize(100,25);
        checkLap->setFixedSize(100,25);
        buttonVoronoi->setFixedSize(0,0);
        buttonDrawCrust->setFixedSize(0,0);
    }
    else {
        check2d->setFixedSize(0,0);
        check->setFixedSize(0,0);
        checkLap->setFixedSize(0,0);
        buttonVoronoi->setFixedSize(100,25);
        buttonDrawCrust->setFixedSize(100,25);

    }
}

// affiche le mesh 3d selon son laplacien
void GLDisplayWidget::laplacians(int arg1)
{
    controlelap=arg1;
    if(arg1==0)
    {
        check->setFixedSize(100,25);
        check2d->setFixedSize(100,25);
        checkCrust->setFixedSize(100,25);
    }
    else {
        check->setFixedSize(0,0);
        check2d->setFixedSize(0,0);
        checkCrust->setFixedSize(0,0);
    }
}

// incrémente l'iterateur de test sur les faces
void GLDisplayWidget::incrementeIterator()
{
    if(!(itonf==m.faces_past_the_end()))
    {
        itonf++;
    }
}


// execute un ajout de point dans une triangulation de delaunay et rends la nouvelle triangulation de delaunay avec l'algorithme basique
void GLDisplayWidget::invertAxis()
{

    double i=distribution(generator);
    double j=distribution(generator);
    std::vector<Vertice> v=mesh2d.getVertice();
    for(unsigned int i=0;i<v.size();i++)
    {
        if(mesh2d.color.size()<=i)
            mesh2d.color.push_back(Point((double)rand()/(double)RAND_MAX,(double)rand()/(double)RAND_MAX,(double)rand()/(double)RAND_MAX));
        if(v[i].p1._x==i && v[i].p1._y==j)
        {
            qDebug()<<"point déja inséré";
            return;
        }
    }
    mesh2d.delaunay_insertion(Point(i,j,0),0);
}

// execute un ajout de point dans une triangulation de delaunay et rends la nouvelle triangulation de delaunay avec l'algoruithme de marche
void GLDisplayWidget::marche()
{

    double i=distribution(generator);
    double j=distribution(generator);
    std::vector<Vertice> v=mesh2d.getVertice();
    for(unsigned int i=0;i<v.size();i++)
    {
        if(mesh2d.color.size()<=i)
            mesh2d.color.push_back(Point((double)rand()/(double)RAND_MAX,(double)rand()/(double)RAND_MAX,(double)rand()/(double)RAND_MAX));
        if(v[i].p1._x==i && v[i].p1._y==j)
        {
            qDebug()<<"point déja inséré";
            return;
        }
    }
    mesh2d.delaunay_insertion(Point(i,j,0),1);
}

// incrémente l'iterateur de test sur les vertices
void GLDisplayWidget::incrementeIteratorV()
{
    if(!(itonv==m.vertice_past_the_end()))
    {
        itonv++;
    }

}

// incrémente le circulateur de test sur les faces
void GLDisplayWidget::incrementeCirculator()
{
        cironf++;
}

// incrémente le circulateur de test sur les vertices
void GLDisplayWidget::incrementeCirculatorV()
{
        cironv++;
}


// affiche l'iterateur de test sur les faces
void GLDisplayWidget::drawIteraror()
{
    glColor3d(1,0,0);
    glBegin(GL_TRIANGLES);
    glPointDraw2(m.getVertex((*itonf).f1).p1);
    glPointDraw2(m.getVertex((*itonf).f2).p1);
    glPointDraw2(m.getVertex((*itonf).f3).p1);
    glEnd();
}

// affiche l'iterateur de test sur les vertices
void GLDisplayWidget::drawIterarorV()
{
    glColor3d(1,0,0);
    glBegin(GL_LINES);
    glPointDraw2((*itonv).p1);
    Point v((*itonv).p1._x+1,(*itonv).p1._y+1,(*itonv).p1._z+1);
    glPointDraw2(v);
    glEnd();
}

// affiche le circulateur de test sur les faces
void GLDisplayWidget::drawCirculator()
{
    glColor3d(0,0,1);
    glBegin(GL_TRIANGLES);
    glPointDraw2(m.getVertex((*cironf).f1).p1);
    glPointDraw2(m.getVertex((*cironf).f2).p1);
    glPointDraw2(m.getVertex((*cironf).f3).p1);
    glEnd();
}

// affiche le circulateur de test sur les vertices
void GLDisplayWidget::drawCirculatorV()
{
    glColor3d(0,0,1);
    glBegin(GL_LINES);
    glPointDraw2((*cironv).p1);
    Point v((*cironv).p1._x+1,(*cironv).p1._y+1,(*cironv).p1._z+1);
    glPointDraw2(v);
    glEnd();
}


void GLDisplayWidget::resizeGL(int width, int height){

    glViewport(0, 0, width, height); //Viewport in the world window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

    updateGL();
}

// - - - - - - - - - - - - Mouse Management  - - - - - - - - - - - - - - - -
// When you click, the position of your mouse is saved
void GLDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL )
    {
        _lastPosMouse = event->pos();
    }
}

// Mouse movement management
void GLDisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - _lastPosMouse.x();
    // int dy = event->y() - lastPosMouse.y();

    if( event != NULL )
    {
        _angle += dx;
        _lastPosMouse = event->pos();

        updateGL();
    }
}

// Mouse Management for the zoom
void GLDisplayWidget::wheelEvent(QWheelEvent *event) {
    QPoint numDegrees = event->angleDelta();
    double stepZoom = 0.25;
    if (!numDegrees.isNull())
    {
      _Z = (numDegrees.x() > 0 || numDegrees.y() > 0) ? _Z + stepZoom : _Z - stepZoom;
    }
}
