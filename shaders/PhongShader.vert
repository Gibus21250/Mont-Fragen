#version 410

uniform mat4 MVP;
uniform mat4 MODEL;
uniform mat4 VIEW;

layout(location = 0) in vec3 position; // le location permet de dire de quel flux/canal on récupère les données (doit être en accord avec le location du code opengl)
layout(location = 1) in vec3 normal;

out vec4 fragPosition;
out vec3 vNormal;

void main(){
    fragPosition = MODEL * vec4(position, 1.);
    vNormal = normal;
    gl_Position = MVP * vec4(position, 1.);
}