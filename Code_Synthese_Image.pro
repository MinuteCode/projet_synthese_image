######################################################################
# Automatically generated by qmake (3.0) Tue May 27 14:31:39 2014
######################################################################

TEMPLATE = app
TARGET = Code_Synthese_Image
INCLUDEPATH += .
QT+= core gui opengl

# Input
HEADERS += glutils.hpp \
           maFenetre.hpp \
           monWidgetGL.hpp \
           objet.h \
           ui_mainwindow.h \
           vec3.hpp \
           vec4.hpp
SOURCES += glutils.cpp \
           maFenetre.cpp \
           main.cpp \
           monWidgetGL.cpp \
           objet.cpp \
           vec3.cpp \
           vec4.cpp

LIBS+= -lGLEW -lGLU -lGL

OTHER_FILES += \
    shader.frag \
    shader.vert
