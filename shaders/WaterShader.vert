#version 410

uniform mat4 MVP;
uniform mat4 MODEL;
uniform float Hauteur_eau;

layout(location = 2) in vec3 position; // le location permet de dire de quel flux/canal on récupère les données (doit être en accord avec le location du code opengl)

void main(){
    gl_Position =  MVP * vec4(position.x,Hauteur_eau,position.z,1.);
}