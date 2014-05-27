#ifndef VEC4_HPP
#define VEC4_HPP
#include <iostream>

struct vec4
{
  vec4():w(),x(),y(),z(){}
  vec4(float w_param, float x_param,float y_param,float z_param): w(w_param),x(x_param),y(y_param),z(z_param){}
  float w,x,y,z;
};

std::ostream& operator<<(std::ostream& stream,vec4& v);

#endif // VEC4_HPP
