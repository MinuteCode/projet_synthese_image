#version 120


varying vec4 position;
varying vec3 vertex_color;

uniform vec3 color;
uniform int is_ground;

//Un Fragment Shader minimaliste
void main (void)
{
    //couleur du fragment
    gl_FragColor = vec4(color,0.0f);

    if(is_ground==1)
    {
        gl_FragColor=vec4(vertex_color,0.0f);
    }

}
