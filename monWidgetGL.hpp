#ifndef MONWIDGETGL_H
#define MONWIDGETGL_H

#include <QTime>
#include <QTimer>
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include "vec3.hpp"

#include <QtOpenGL/QGLWidget>


class monWidgetGL : public QGLWidget
{
    Q_OBJECT
public:
    monWidgetGL(const QGLFormat& format,QGLWidget *parent = 0);
    ~monWidgetGL();
    void afficher();
    void wireframe(bool est_actif);
    void detection_collision();
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);

protected:

    void initializeGL();
    void paintGL();
    void resizeGL();

private slots:
    void update_timer();

private:

    GLuint shader_program_id;
    GLuint vbo;
    GLuint vbo2;
    GLuint vbo3;
    GLuint ibo;
    GLuint ibo2;
    GLuint ibo3;
    GLuint nbo;
    QTimer timer;

    bool etat_affichage;
};

#endif // MONWIDGETGL_H
