#version 410

//light
uniform vec3 l_position;
uniform vec3 l_intensity;
uniform float l_ambientCoefficient;
uniform float l_attenuation;

uniform vec3 cameraPosition;

uniform mat4 MODEL;

//in
in vec4 fragPosition;
in vec3 vNormal;
in vec3 fragColor;

out vec4 finalColor;

void main() {
    vec3 directLight = vec3(0, -1/sqrt(5./4), -1/2*sqrt(5./4));
    vec3 nvNormal = normalize(vNormal);

    nvNormal = transpose(inverse(mat3 (MODEL))) * nvNormal;
    nvNormal = normalize(nvNormal);
    
    vec3 toLight = -directLight;

    //Recupérer vecteur en direction de la lampe
    /*
    vec3 toLight = directLight - fragPosition.xyz;
    float dist2 = dot(toLight, toLight);
    toLight = normalize(toLight);
    */

    vec3 toCamera = cameraPosition.xyz - fragPosition.xyz;
    toCamera = normalize(toCamera);

    vec3 rtolight = reflect(-toLight, nvNormal);
    rtolight = normalize(rtolight);

    vec3 colorA = vec3(0.,0.,0.);
    vec3 colorD = vec3(0.,0.,0.);
    vec3 colorS = vec3(0.,0.,0.);

    if(fragPosition.y > 22)
    {
        colorA = l_ambientCoefficient * vec3(255/255.,250/255.,250/255.);
        colorD = max(0, dot(nvNormal, toLight)) * l_intensity * vec3(255/255.,250/255.,250/255.);
        colorS = pow(max(0, dot(toCamera, rtolight)), 5) *  vec3(255/255.,250/255.,250/255.);
    }
    else
    {
        colorA = l_ambientCoefficient * vec3(63/255.,34/255.,4/255.);
        colorD = max(0, dot(nvNormal, toLight)) * l_intensity * vec3(63/255.,34/255.,4/255.);
        colorS = pow(max(0, dot(toCamera, rtolight)), 5) * vec3(63/255.,34/255.,4/255.);
    }

    finalColor = vec4(colorA, 1.) + vec4(colorD, 1.) + vec4(colorS,1.);
}