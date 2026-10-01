#version 330

in vec3 o_normal;
in vec3 worldPos;

uniform vec3 uColor;
uniform vec3 cameraPos;

out vec4 finalColor;

float saturation(float v)
{
    return clamp(v, 0.0, 1.0);
}

vec3 saturation(vec3 v)
{
    return clamp(v, 0.0, 1.0);
}

float remap(float x, float a, float b, float c, float d)
{
    return mix(c, d, (x - a) / (b - a));
}

void main()
{
    //light
    vec3 normals = normalize(o_normal);
    vec3 lightDir = normalize(vec3(0.0, 0.5, 1.0));
    float diffFactor = dot(normals, lightDir);
    float st = saturation(diffFactor);
    
    //color
    vec3 color = pow(uColor, vec3(2.2));

    //hemi
    vec3 skyColor = pow(vec3(0.5, 0.7, 1.0), vec3(2.2));
    vec3 groundColor = pow(vec3(0.65, 0.50, 0.38), vec3(2.2));
    float hemiMix = remap(normals.y, -1, 1, 0, 1);
    vec3 hemi = mix(groundColor, skyColor, hemiMix);
    vec3 hemiWColor = hemi * color;
    
    //specular
    vec3 viewDir = normalize(cameraPos - worldPos);
    vec3 r = normalize(reflect(-lightDir, normals));
    float phongValue = pow(max(0.0, dot(viewDir, r)), 128.0);
    vec3 specular = vec3(phongValue) * vec3(1,1,1);

    //final
    vec3 diffuseColor = color * st + specular;
    vec3 result = pow(diffuseColor + hemiWColor, vec3(1.0 / 2.2));
    finalColor = vec4(result, 1.0);
}
