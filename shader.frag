#version 120


varying vec4 position;
varying vec3 vertex_color;

//Un Fragment Shader minimaliste
void main (void)
{
    //couleur du fragment
    gl_FragColor = vec4(vertex_color,0.0f);

}
