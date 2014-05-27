
#include <QApplication>
#include <QtOpenGL/QGLWidget>
#include <iostream>

#include "maFenetre.hpp"




int main(int argc,char *argv[])
{
  QApplication app(argc,argv);

  maFenetre fenetre;

  std::cout<<"Affichage de la fenetre"<<std::endl;
  fenetre.show();

  std::cout<<"Lancement de l'application principale"<<std::endl;
  return app.exec();
}
