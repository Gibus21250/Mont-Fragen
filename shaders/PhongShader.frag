#version 410

//light
uniform vec3 l_position;
uniform vec3 l_intensity;
uniform float l_ambientCoefficient;
uniform float l_attenuation;

uniform vec3 snow_color;
uniform vec3 snow_diffuse;
uniform vec3 snow_specular;

uniform vec3 stone_color;
uniform vec3 stone_diffuse;
uniform vec3 stone_specular;

uniform vec3 grass_color;
uniform vec3 grass_diffuse;
uniform vec3 grass_specular;

uniform vec3 sand_color;
uniform vec3 sand_diffuse;
uniform vec3 sand_specular;

uniform vec3 Map1;
uniform vec3 Map2;

uniform vec3 cameraPosition;

uniform mat4 MODEL;

float inter;
float line;

//in
in vec4 fragPosition;
in vec3 vNormal;
in vec3 fragColor;

out vec4 finalColor;

float random (vec2 st) {
    return fract(sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123);
}

float interpole(float max, float min, float val) {
    return max * (1.0f - val) + max * val;
}

float normalise(float valeur, float min, float max) {
    return (valeur - min) / (max - min);
}

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

    if(fragPosition.y <= Map1.z+1) //espace sous l eau sand
    {
        colorA = l_ambientCoefficient *  vec3(sand_color.x/255.,sand_color.y/255.,sand_color.z/255.);
        colorD = max(0, dot(nvNormal, toLight)) * l_intensity * vec3(sand_diffuse.x/255.,sand_diffuse.y/255.,sand_diffuse.z/255.);
        colorS = pow(max(0, dot(toCamera, rtolight)), 5) * vec3(sand_specular.x/255.,sand_specular.y/255.,sand_specular.z/255.);
    }

    else if(fragPosition.y >= Map1.x) //espace neige (sommets) 
    {
        colorA = l_ambientCoefficient * vec3(snow_color.x/255.,snow_color.y/255.,snow_color.z/255.);
        colorD = max(0, dot(nvNormal, toLight)) * l_intensity * vec3(snow_diffuse.x/255.,snow_diffuse.y/255.,snow_diffuse.z/255.);
        colorS = pow(max(0, dot(toCamera, rtolight)), 5) *  vec3(snow_specular.x/255.,snow_specular.y/255.,snow_specular.z/255.);
    }

    else if( (fragPosition.y < Map1.x && fragPosition.y > Map1.y)) // espace sous les sommets
    {

        inter = interpole(Map1.x,Map1.y,fragPosition.y);
        line = normalise(inter,Map1.y,Map1.x);

        if( random(fragPosition.xz) < 0.8)
        {
            colorA = l_ambientCoefficient * vec3(snow_color.x/255.,snow_color.y/255.,snow_color.z/255.);
            colorD = max(0, dot(nvNormal, toLight)) * l_intensity * vec3(snow_diffuse.x/255.,snow_diffuse.y/255.,snow_diffuse.z/255.);
            colorS = pow(max(0, dot(toCamera, rtolight)), 5) *  vec3(snow_specular.x/255.,snow_specular.y/255.,snow_specular.z/255.);
        }
        else
        {
            colorA = l_ambientCoefficient * vec3(stone_color.x/255.,stone_color.y/255.,stone_color.z/255.);
            colorD = max(0, dot(nvNormal, toLight)) * l_intensity * vec3(stone_diffuse.x/255.,stone_diffuse.y/255.,stone_diffuse.z/255.);
            colorS = pow(max(0, dot(toCamera, rtolight)), 5) *  vec3(stone_specular.x/255.,stone_specular.y/255.,stone_specular.z/255.);
        }
        
    }

    else if((fragPosition.y < Map1.y)) // espace plaines
    {
        inter = 1- interpole(Map1.x,Map1.y,fragPosition.y);
        line = 1- normalise(inter,Map1.x,Map1.y);
        if(random(fragPosition.xz) < 0.8)
        {
            colorA = l_ambientCoefficient * vec3(grass_color.x/255.,grass_color.y/255.,grass_color.z/255.);
            colorD = max(0, dot(nvNormal, toLight)) * l_intensity * vec3(grass_diffuse.x/255.,grass_diffuse.y/255.,grass_diffuse.z/255.);
            colorS = pow(max(0, dot(toCamera, rtolight)), 5) *  vec3(grass_specular.x/255.,grass_specular.y/255.,grass_specular.z/255.);
        }
        else
        {
            colorA = l_ambientCoefficient * vec3(stone_color.x/255.,stone_color.y/255.,stone_color.z/255.);
            colorD = max(0, dot(nvNormal, toLight)) * l_intensity * vec3(stone_diffuse.x/255.,stone_diffuse.y/255.,stone_diffuse.z/255.);
            colorS = pow(max(0, dot(toCamera, rtolight)), 5) *  vec3(stone_specular.x/255.,stone_specular.y/255.,stone_specular.z/255.);
        }

    }

    else
    {
        colorA = l_ambientCoefficient * vec3(stone_color.x/255.,stone_color.y/255.,stone_color.z/255.);
        colorD = max(0, dot(nvNormal, toLight)) * l_intensity * vec3(stone_diffuse.x/255.,stone_diffuse.y/255.,stone_diffuse.z/255.);
        colorS = pow(max(0, dot(toCamera, rtolight)), 5) *  vec3(stone_specular.x/255.,stone_specular.y/255.,stone_specular.z/255.);
    }


    finalColor = vec4(colorA, 1.) + vec4(colorD, 1.) + vec4(colorS, 1.);
}