#-------------------------------------------------
#
# Project created by QtCreator 2018-08-28T10:55:17
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mesh_Computational_Geometry
TEMPLATE = app

SOURCES += main.cpp\
    circulator_on_faces.cpp \
    circulator_on_vertices.cpp \
    iterator_on_faces.cpp \
    iterator_on_vertices.cpp \
    laplacian.cpp \
        mainwindow.cpp \
    gldisplaywidget.cpp \
    mesh.cpp \
    utilitaire.cpp

HEADERS  += mainwindow.h \
    circulator_on_faces.h \
    circulator_on_vertices.h \
    gldisplaywidget.h \
    iterator_on_faces.h \
    iterator_on_vertices.h \
    laplacian.h \
    mesh.h \
    utilitaire.h

FORMS    += mainwindow.ui

#---- Comment the following line on MacOS
LIBS = -lGLU

#---- Uncomment the following line on Windows
#LIBS += -lglu32
L#IBS += -lOpengl32

