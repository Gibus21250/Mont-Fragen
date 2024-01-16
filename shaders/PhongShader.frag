#version 410

//light
uniform vec3 l_position;
uniform vec3 l_intensity;
uniform float l_ambientCoefficient;
uniform float l_attenuation;

uniform vec3 cameraPosition;

//texture
uniform mat4 MODEL;

//in
in vec4 fragPosition;
in vec3 vNormal;

out vec4 finalColor;

void main() {

    vec3 nvNormal = normalize(vNormal);
    nvNormal = transpose(inverse(mat3 (MODEL))) * nvNormal;
    nvNormal = normalize(nvNormal);
    //Recupérer vecteur en direction de la lampe
    vec3 toLight = l_position - fragPosition.xyz;
    float dist2 = dot(toLight, toLight);
    toLight = normalize(toLight);

    vec3 toCamera = cameraPosition.xyz - fragPosition.xyz;
    toCamera = normalize(toCamera);

    vec3 rtolight = reflect(-toLight, nvNormal);
    rtolight = normalize(rtolight);
    
    
    vec3 colorA = l_ambientCoefficient * vec3(0, 0, 1);
    vec3 colorD = max(0, dot(nvNormal, toLight)) * l_intensity * vec3(0.,1., 0.) * 1/dist2;
    vec3 colorS = pow(max(0, dot(toCamera, rtolight)), 5) * vec3(1.,0.,0.) * 1/dist2;

    //finalColor = vec4(colorA, 1.) + vec4(colorD, 1.)+ vec4(colorS, 1);
    finalColor = vec4(.55,0.25,0.1, 1.);
}