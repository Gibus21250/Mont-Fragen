#version 410

uniform mat4 MVP;
uniform mat4 MODEL;

layout(location = 2) in vec3 position; // le location permet de dire de quel flux/canal on récupère les données (doit être en accord avec le location du code opengl)

out vec4 fragPosition;

void main(){
    fragPosition = MODEL * vec4(position, 1.);
    gl_Position = MVP * vec4(position, 1.);
}