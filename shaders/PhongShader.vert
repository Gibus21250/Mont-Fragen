#version 410

uniform mat4 MVP;
uniform mat4 MODEL;
uniform mat4 VIEW;

layout(location = 0) in vec3 position; // le location permet de dire de quel flux/canal on récupère les données (doit être en accord avec le location du code opengl)
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colors;

out vec4 fragPosition;
out vec3 vNormal;
out vec3 fragColor;

void main(){
    fragPosition = MODEL * vec4(position, 1.);
    fragColor = colors;
    vNormal = normal;
    gl_Position = MVP * vec4(position, 1.);
}