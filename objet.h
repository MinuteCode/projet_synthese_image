#ifndef OBJET_H
#define OBJET_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "glutils.hpp"
#include <GL/glu.h>
#include <cstdlib>

#include "vec3.hpp"
#include "vec4.hpp"

class Objet
{
private:
    std::vector<vec3> vecteur_sommet;
    std::vector<GLushort> vecteur_face;
    std::vector<vec3> vecteur_normales;


public:
    Objet();
    std::vector<vec3> getVecteurSommets() const;
    std::vector<GLushort> getVecteurFaces() const;
    std::vector<vec3> getVecteurNormales() const;

    void load_obj(std::string nomFichier);
};

#endif // OBJET_H
