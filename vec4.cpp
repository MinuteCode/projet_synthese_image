#include "vec4.hpp"

std::ostream& operator<<(std::ostream& stream,vec4& v)
{
    stream<<v.x<<"|"<<v.y<<"|"<<v.z<<"|"<<v.w<<std::endl;
    return stream;
}
