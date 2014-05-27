#version 120

varying vec4 vertex_3d_original;
varying vec4 vertex_3d_modelview;
varying vec3 vertex_color;

varying vec4 position;

uniform float time;
uniform vec3 translation;
uniform float Angle;
uniform int rotation;

//Un Vertex Shader minimaliste
void main (void)
{
    position=gl_Vertex;

    mat4 RotationMatrix = mat4( cos(Angle),0,-sin( Angle ), 0.0,
                                0         ,1,0            ,0   ,
                                sin(Angle) ,0,cos( Angle ) , 0.0,
                                0.0,   0.0,0,          1.0 );

    position*=RotationMatrix;
    position += vec4(translation.x,translation.y,translation.z,0.0f);


    /*mat4 TranslationMatrix = mat4(1.0,0.0,0.0,translation.x,
                                  0.0,1.0,0.0,translation.y,
                                  0.0,0.0,1.0,translation.z,
                                  0.0,0.0,0.0,1.0);*/
    //Projection du sommet
    vertex_color=gl_Vertex.xyz;
    vertex_3d_original=gl_Vertex;
    vertex_3d_modelview=gl_ModelViewMatrix*gl_Vertex;
    gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*position;

}
