#version 410

uniform mat4 MVP;
uniform float Hauteur_eau;
uniform float t;

layout(location = 0) in vec3 position;

void main(){
    float biasx = 0.5 * (cos(t/6 + position.x)+ sin(t/8 + position.y))/10;
    float biasy = 0.6 * (cos(t/6 + position.y)+ sin(t/8 + position.x))/10;
    gl_Position =  MVP * vec4(position.x,(Hauteur_eau + (biasx + biasy)/2 ),position.z,1.);
}