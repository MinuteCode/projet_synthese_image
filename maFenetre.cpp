#include "maFenetre.hpp"
#include "monWidgetGL.hpp"

#include <iostream>

#include "ui_mainwindow.h"

maFenetre::maFenetre(QWidget *parent)
    :QMainWindow(parent),ui(new Ui::MainWindow)
{
    std::cout<<"Generation de la fenetre"<<std::endl;

    ui->setupUi(this);

    std::cout<<"Cree Widget OpenGL"<<std::endl;

    QGLFormat qglFormat;
    qglFormat.setVersion(1,2);
    glWidget=new monWidgetGL(qglFormat);
    ui->layout_scene->addWidget(glWidget);



    connect(ui->quitter,SIGNAL(clicked()),this,SLOT(action_quitter()));
    connect(ui->afficher,SIGNAL(clicked()),this,SLOT(action_afficher()));
    connect(ui->checkBox,SIGNAL(clicked()),this,SLOT(action_wireframe()));

}

maFenetre::~maFenetre()
{
    std::cout<<"Destruction fenetre"<<std::endl;
    if(ui!=NULL)
    {
        delete ui;
        ui=NULL;
    }
}

void maFenetre::action_quitter()
{
    close();
}

void maFenetre::action_afficher()
{
    glWidget->afficher();
}

void maFenetre::action_wireframe()
{
    bool etat_wireframe=ui->checkBox->isChecked();
    glWidget->wireframe(etat_wireframe);
}

