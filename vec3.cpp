#include "vec3.hpp"



std::ostream& operator<<(std::ostream& stream,vec3& v)
{
    stream<<v.x<<"|"<<v.y<<"|"<<v.z<<std::endl;
    return stream;
}

// Produit vectoriel pour calculer les normales
vec3 vec3::crossproduct(vec3 &v)
{
    vec3 result;
    result.x=y*v.z-z*v.y;
    result.y=z*v.x-x*v.z;
    result.z=x*v.y-y*v.x;
    return result;
}

vec3 vec3::operator-(vec3& v2)
{
    x-=v2.x;
    y-=v2.y;
    z-=v2.z;
    return *this;
}
