#version 410

uniform mat4 MVP;
uniform mat4 MODEL;
uniform mat4 VIEW;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colors;

in float floater;

out vec4 fragPosition;
out vec3 vNormal;
out vec3 fragColor;

void main(){
    fragPosition = vec4(position, 1.);
    fragColor = colors;
    vNormal = normal;
    gl_Position = MVP * vec4(position, 1.);
}