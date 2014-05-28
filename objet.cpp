#include "objet.h"
#include <exception>


Objet::Objet()
{
}


std::vector<vec3>& Objet::getVecteurSommets()
{
    return vecteur_sommet;
}

std::vector<vec3> Objet::getVecteurNormales() const
{
    return vecteur_normales;
}

std::vector<GLushort> Objet::getVecteurFaces() const
{
    return vecteur_face;

}

void Objet::load_obj(std::string nomFichier)
{
    // Ouverture du fichier
    std::fstream fichier(nomFichier.c_str(), std::fstream::in);

    if(!fichier)
    {
        std::cerr<<"Cannot open "<<fichier<<std::endl;
        exit(1);
    }

    std::cout<<"Ouverture .obj réalisée avec succès"<<std::endl;
    std::cout<<"Chargement "<<nomFichier<<"..."<<std::endl;
    std::string line;


    while(getline(fichier, line)) // Lecture de chaque ligne
    {
        // ajout d'un sommet dans la liste de sommets
        if(line.substr(0,2)=="v ")
        {
            std::stringstream sstream(line.substr(2));
            vec3 v;
            sstream>>v.x;
            v.x=v.x;
            sstream>>v.y;
            v.y=v.y;
            sstream>>v.z;
            v.z=v.z;
            vecteur_sommet.push_back(v);
        }

        else if(line.substr(0,3)=="vn ")
        {
            std::stringstream sstream(line.substr(3));
            vec3 v;
            sstream>>v.x;
            v.x=v.x;
            sstream>>v.y;
            v.y=v.y;
            sstream>>v.z;
            v.z=v.z;
            vecteur_normales.push_back(v);
        }

        // Chargement d'une face et des normales
        else if (line.substr(0,2)=="f ")
        {
            int found=line.find("//");
            bool normal=false;
            if (found>0)
                normal=true;

            while(found>0)
            {
                line.replace(found,2," ",0,1);
                found=line.find("//");
            }

            // Ajout des indices à la liste d'indices
            std::stringstream sstream(line.substr(2));
            if(normal)
            {
                GLushort a,b,c,n1,n2,n3;
                sstream>>a;
                sstream>>n1;
                sstream>>b;
                sstream>>n2;
                sstream>>c;
                sstream>>n3;
                vecteur_face.push_back(--a);vecteur_face.push_back(--b);vecteur_face.push_back(--c);// dans le point obj commence à 1 ici à 0
            }
            else
            {
                GLushort a,b,c;
                sstream>>a;
                sstream>>b;
                sstream>>c;
                vecteur_face.push_back(--a);vecteur_face.push_back(--b);vecteur_face.push_back(--c);// dans le point obj commence à 1 ici à 0
            }

        }

        // Commentaires et lignes vides
        else if (line.substr(0,2)=="# ") { /*Ignore la ligne car commentaire*/ }
        else { /*Ignore la ligne car information non prise en compte*/ }

    }

    /*Calcul des normales au plan s'est avéré inutile puisque nous avons implémenté la lecture des normales dans le fichier obj*/
    /*vecteur_normales.resize(vecteur_face.size(),vec3(0.0f,0.0f,0.0f));
    for (unsigned int i=0;i<vecteur_face.size();i+=3)
    {
        std::cout<<"i : "<<i<<std::endl;
        GLushort ia=vecteur_face[i];
        std::cout<<ia<<std::endl;
        GLushort ib=vecteur_face[i+1];
        std::cout<<ib<<std::endl;
        GLushort ic=vecteur_face[i+2];
        std::cout<<ic<<std::endl;

        vec3 v1,v2,vec_normal;
        v1=vecteur_sommet[ib]-vecteur_sommet[ia];
        v2=vecteur_sommet[ic]-vecteur_sommet[ia];
        vec_normal=v1.crossproduct(v2);
        std::cout<<"vec_normal : "<<vec_normal<<std::endl;
        vec_normal.x/=std::sqrt(std::pow(vec_normal.x,2)+std::pow(vec_normal.y,2)+std::pow(vec_normal.z,2));
        vec_normal.y/=std::sqrt(std::pow(vec_normal.x,2)+std::pow(vec_normal.y,2)+std::pow(vec_normal.z,2));
        vec_normal.z/=std::sqrt(std::pow(vec_normal.x,2)+std::pow(vec_normal.y,2)+std::pow(vec_normal.z,2));
        std::cout<<"vec_normal normalisé : "<<vec_normal<<std::endl;
        //vecteur_normales[ia]=vecteur_normales[ib]=vecteur_normales[ic]=vec_normal;
        vecteur_sommet.push_back(vec_normal);

    }*/

    fichier.close();
    if(fichier)
    {
        std::cout<<"Fichier non fermé !"<<std::endl;
    }
    else std::cout<<"Fichier fermé avec succès"<<std::endl;

}





