#ifndef VEC3_HPP
#define VEC3_HPP
#include <iostream>

struct vec3
{
  vec3():x(),y(),z(){}
  vec3(float x_param,float y_param,float z_param):x(x_param),y(y_param),z(z_param){}

  vec3 crossproduct(vec3& v);
  vec3 operator-(vec3& v2);
  float x,y,z;
};

std::ostream& operator<<(std::ostream& stream,vec3& v);


#endif // VEC3_HPP
