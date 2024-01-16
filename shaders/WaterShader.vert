#version 410

uniform mat4 MVP;
uniform mat4 MODEL;
uniform float Hauteur_eau;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vnormal;

void main(){
    gl_Position =  MVP * vec4(position.x,Hauteur_eau,position.z,1.);
}