#include "monWidgetGL.hpp"
#include "objet.h"
#include "glutils.hpp"

#include <GL/glu.h>
#include <GL/glew.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>

#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>

struct DirectionalLight{
    vec3 color;
    float AmbientIntensity;
};

Objet robot,sol,cylindre;

DirectionalLight dirLight;

float rotation_x=0;
float rotation_y=0;
float rotation_z=0;
float Angle=0;

//paramètres gestion camera
float x_clicked;
float y_clicked;
float x_translate=0.0f;
float y_translate=1.0f;
float z_translate=-2.0f;
float x_translation_old=0.0f;
float y_translation_old=0.0f;
float z_translate_old=0.0f;

//Point Of Interest : point regardé par la caméra
vec3 POI(x_translate,y_translate,z_translate+2.0f);

bool left_button=false;
bool right_button=false;
int rotation=0;


float time1=0.0f;
vec3 translation(0.3f,0.0f,0.0f);
vec3 pos_robot;

//identifiant du shsader
GLuint shader_program_id;
GLint m_dirLightAmbient;
GLint m_dirLightColor;


monWidgetGL::monWidgetGL(const QGLFormat& format,QGLWidget *parent) :
    QGLWidget(format,parent)
{
    etat_affichage=true;
}

monWidgetGL::~monWidgetGL()
{}

//******************************************************************//

//******************************************************************//

void monWidgetGL::initializeGL()
{
    std::cout<<"appel monWidgetGL::initializeGL()"<<std::endl;
    glewInit();
    glLineWidth(2.0f);

    std::cout<<"Initialisation timer"<<std::endl;
    connect(&timer, SIGNAL(timeout()), this, SLOT(update_timer()));
    timer.start(16);
    std::cout<<"Timer initialisé"<<std::endl;

    robot.load_obj("Objects/Robot_normals_no_plan.obj");
    std::cout<<"taille du vecteur sommets : "<<robot.getVecteurSommets().size()<<std::endl;
    std::cout<<"taille du vecteur faces : "<<robot.getVecteurFaces().size()<<std::endl;
    std::cout<<"taille du vecteur normales : "<<robot.getVecteurNormales().size()<<std::endl;

    for(unsigned int i=0;i<robot.getVecteurSommets().size();i++)
    {
        pos_robot.x+=robot.getVecteurSommets().at(i).x;
        pos_robot.y+=robot.getVecteurSommets().at(i).y;
        pos_robot.z+=robot.getVecteurSommets().at(i).z;
    }
    pos_robot.x/=robot.getVecteurSommets().size();
    pos_robot.y/=robot.getVecteurSommets().size();
    pos_robot.z/=robot.getVecteurSommets().size();

    sol.load_obj("Objects/sol2.obj");
    cylindre.load_obj("Objects/cylindre.obj");

    for(unsigned int i=0;i<sol.getVecteurSommets().size();i++)
    {
        sol.getVecteurSommets().at(i)*=5.0f;
    }

    glClearColor(0.3,0.3,0.3, 0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);


    /*******************************************************************/
    /*              Création vertex buffer object Robot                */
    /*******************************************************************/
    glGenBuffers(1,&vbo); PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,sizeof(vec3)*robot.getVecteurSommets().size(),robot.getVecteurSommets().data(),GL_STATIC_DRAW); PRINT_OPENGL_ERROR();
    PRINT_OPENGL_ERROR();

    /*******************************************************************/
    /*              Création index buffer object Robot                 */
    /*******************************************************************/
    glGenBuffers(1,&ibo); PRINT_OPENGL_ERROR();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort)*robot.getVecteurFaces().size(),robot.getVecteurFaces().data(),GL_STATIC_DRAW);

    /*******************************************************************/
    /*              Création vertex buffer object Sol                  */
    /*******************************************************************/
    glGenBuffers(1,&vbo2); PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo2); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,sizeof(vec3)*sol.getVecteurSommets().size(),sol.getVecteurSommets().data(),GL_STATIC_DRAW); PRINT_OPENGL_ERROR();
    PRINT_OPENGL_ERROR();

    /*******************************************************************/
    /*              Création index buffer object Sol                   */
    /*******************************************************************/
    glGenBuffers(1,&ibo2); PRINT_OPENGL_ERROR();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort)*sol.getVecteurFaces().size(),sol.getVecteurFaces().data(),GL_STATIC_DRAW);

    /*******************************************************************/
    /*              Création vertex buffer object Cylindre             */
    /*******************************************************************/
    glGenBuffers(1,&vbo3); PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo3); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,sizeof(vec3)*cylindre.getVecteurSommets().size(),cylindre.getVecteurSommets().data(),GL_STATIC_DRAW); PRINT_OPENGL_ERROR();
    PRINT_OPENGL_ERROR();

    /*******************************************************************/
    /*              Création index buffer object Cylindre              */
    /*******************************************************************/
    glGenBuffers(1,&ibo3); PRINT_OPENGL_ERROR();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort)*cylindre.getVecteurFaces().size(),cylindre.getVecteurFaces().data(),GL_STATIC_DRAW);

    // Chargement du shader
    shader_program_id = read_shader("shader.vert", "shader.frag");PRINT_OPENGL_ERROR();

    glEnable(GL_DEPTH_TEST); PRINT_OPENGL_ERROR();
    setFocusPolicy(Qt::StrongFocus);
}



void monWidgetGL::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x_translate,y_translate,z_translate,x_translate,y_translate,0.0f,0.0f,1.0f,0.0f);
    glRotatef(POI.y, 1.0f, 0.0f, 0.0f);
    glRotatef(POI.x, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation_z, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f,1.5f,0.1f,100.0f);
    //glUniform1f(get_uni_loc(shader_program_id,"time"),time1);


    /*******************************************************************/
    /*                       Affichage robot                           */
    /*******************************************************************/
    glBindBuffer(GL_ARRAY_BUFFER,vbo); PRINT_OPENGL_ERROR();
    glEnableClientState(GL_VERTEX_ARRAY); PRINT_OPENGL_ERROR();
    glVertexPointer(3, GL_FLOAT, 0, 0); PRINT_OPENGL_ERROR();


    /*glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();
    glNormalPointer(GL_UNSIGNED_INT, 0, robot.getVecteurNormales().size()); PRINT_OPENGL_ERROR();*/

    glUniform1f(get_uni_loc(shader_program_id,"Angle"),Angle);
    glUniform3f(get_uni_loc(shader_program_id,"translation"),translation.x,translation.y,translation.z);
    //glUniform1i(get_uni_loc(shader_program_id,"rotation"),rotation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glDrawElements(GL_TRIANGLES,robot.getVecteurFaces().size(),GL_UNSIGNED_SHORT,0);



    /*******************************************************************/
    /*                       Affichage sol                             */
    /*******************************************************************/
    glBindBuffer(GL_ARRAY_BUFFER,vbo2); PRINT_OPENGL_ERROR();
    glEnableClientState(GL_VERTEX_ARRAY); PRINT_OPENGL_ERROR();
    glVertexPointer(3, GL_FLOAT, 0, 0); PRINT_OPENGL_ERROR();

    glUniform1f(get_uni_loc(shader_program_id,"Angle"),0*M_PI/180); //RAZ rotation pour que le sol reste immobile
    glUniform3f(get_uni_loc(shader_program_id,"translation"),0.0,0.0,0.0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo2);
    glDrawElements(GL_TRIANGLES,sol.getVecteurFaces().size(),GL_UNSIGNED_SHORT,0);

    /*******************************************************************/
    /*                       Affichage cylindre                        */
    /*******************************************************************/
    glBindBuffer(GL_ARRAY_BUFFER,vbo3); PRINT_OPENGL_ERROR();
    glEnableClientState(GL_VERTEX_ARRAY); PRINT_OPENGL_ERROR();
    glVertexPointer(3, GL_FLOAT, 0, 0); PRINT_OPENGL_ERROR();

    glUniform1f(get_uni_loc(shader_program_id,"Angle"),0*M_PI/180); //RAZ rotation pour que le sol reste immobile
    glUniform3f(get_uni_loc(shader_program_id,"translation"),5.0,0.0,7.0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo3);
    glDrawElements(GL_TRIANGLES,cylindre.getVecteurFaces().size(),GL_UNSIGNED_SHORT,0);

}

void monWidgetGL::resizeGL()
{
}


void monWidgetGL::afficher()
{
    etat_affichage=!etat_affichage;

    std::cout<<etat_affichage<<std::endl;

     updateGL();

}
void monWidgetGL::wireframe(bool est_actif)
{
    if(est_actif==true)
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);

    updateGL();
}

void monWidgetGL::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {

    //****************************************************************//
    //*****Contrôle de la caméra avec les touches directionnelles*****//
    //****************************************************************//

    case Qt::Key_Up:
        POI.y-=3.0f;
        break;

    case Qt::Key_Down:
        POI.y+=3.0f;
        break;

    case Qt::Key_D:
        Angle+=0.1;
        break;

    case Qt::Key_Q:
        Angle-=0.1;
        break;

    case Qt::Key_1:
        rotation_z+=3.0f;
        break;
    case Qt::Key_2:
        rotation_z-=3.0f;
        break;

//****************************************************************//
//**********Contrôle du robot avec les touches zsqd***************//
//****************************************************************//

    case Qt::Key_Z:
        for( unsigned int i=0;i<sol.getVecteurSommets().size();i++)
        {
            if(pos_robot.x>=25 || pos_robot.x<-16 || pos_robot.z>20 || pos_robot.z<-20)
            {
                std::cout<<"collision"<<std::endl;
                QMessageBox* messageBox=new QMessageBox(this);
                messageBox->setText("Une collision a ete detectee");
                messageBox->exec();
                translation.x=0.0f;
                translation.z=0.0f;
                pos_robot.x=0.0f;
                pos_robot.z=0.0f;
            }
            else if(std::sqrt(std::pow(pos_robot.x-5,2)+std::pow(pos_robot.z-7,2))<2.5)
            {
                QMessageBox* messageBox=new QMessageBox(this);
                messageBox->setText("Une collision avec le cylindre a ete detectee");
                messageBox->exec();
                translation.x=0.0f;
                translation.z=0.0f;
                pos_robot.x=0.0f;
                pos_robot.z=0.0f;
            }
            else
            {
                translation.z+=0.03*sin(Angle);
                translation.x+=0.03*cos(Angle);
                pos_robot.x+=0.03*cos(Angle);
                pos_robot.z+=0.03*sin(Angle);
            }
        }
        std::cout<<pos_robot.x<<","<<pos_robot.z<<std::endl;
        break;

    case Qt::Key_S:
        for( unsigned int i=0;i<sol.getVecteurSommets().size();i++)
        {
            if(pos_robot.x>=25 || pos_robot.x<-16 || pos_robot.z>20 || pos_robot.z<-20)
            {
                std::cout<<"collision"<<std::endl;
            }
            else if(std::sqrt(std::pow(pos_robot.x-5,2)+std::pow(pos_robot.z-7,2))<2.5)
            {
                QMessageBox* messageBox=new QMessageBox(this);
                messageBox->setText("Une collision avec le cylindre a ete detectee");
                messageBox->exec();
                translation.x=0.0f;
                translation.z=0.0f;
                pos_robot.x=0.0f;
                pos_robot.z=0.0f;
            }
            else
            {
                translation.z-=0.03*sin(Angle);
                translation.x-=0.03*cos(Angle);
                pos_robot.x-=0.03*cos(Angle);
                pos_robot.z-=0.03*sin(Angle);
            }
        }
        std::cout<<pos_robot.x<<","<<pos_robot.z<<std::endl;
        break;

    case Qt::Key_Plus:
        m_dirLightAmbient+=0.5f;
        break;
    default:
        break;
    }

}

void monWidgetGL::wheelEvent(QWheelEvent* event)
{
    if (event->delta()>=0)
    {
        z_translate+=1.0f;

    }
    else
    {
        z_translate-=1.0f;
    }

}

void monWidgetGL::update_timer()
{
    updateGL();
}

void monWidgetGL::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        left_button=true;
    else if(event->button()==Qt::RightButton)
        right_button=true;

    x_clicked=event->x();
    y_clicked=event->y();

}

void monWidgetGL::mouseReleaseEvent(QMouseEvent *)
{
    left_button=false;
    right_button=false;
}

void monWidgetGL::mouseMoveEvent(QMouseEvent *event)
{
    if(left_button)
    {
            x_translate=event->x()-x_clicked;
            y_translate=event->y()-y_clicked;
            x_translate/=20;
            y_translate/=20;
    }
    else if(right_button)
    {
        POI.x=(event->x()-x_clicked)/2;
        POI.y=(event->y()-y_clicked)/2;
    }

}

void monWidgetGL::detection_collision()
{
    for(unsigned int i=0;i<robot.getVecteurSommets().size();i++)
    {
        for(unsigned int j=0;j<sol.getVecteurSommets().size();j++)
        {
            if(std::abs(robot.getVecteurSommets().at(i).x-sol.getVecteurSommets().at(j).x)<0.1)
            {
                std::cout<<std::abs(robot.getVecteurSommets().at(i).x-sol.getVecteurSommets().at(j).x)<<std::endl;
                exit(0);
            }
        }
    }
}


