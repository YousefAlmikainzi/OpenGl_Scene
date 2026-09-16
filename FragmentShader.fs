#version 330

in vec3 o_normal;

uniform vec3 uColor;

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
    vec3 normals = normalize(o_normal);
    vec3 dir = normalize(vec3(0.0, 0.5, 1.0));
    float diffFactor = dot(normals, dir);
    float st = saturation(diffFactor);
    //color
    vec3 color = pow(uColor, vec3(2.2));
    //ambient
    //vec3 ambient = color * 0.25;
    //hemi
    vec3 skyColor = pow(vec3(0.5, 0.7, 1.0), vec3(2.2));
    vec3 groundColor = pow(vec3(0.65, 0.50, 0.38), vec3(2.2));

    float hemiMix = remap(normals.y, -1, 1, 0, 1);
    vec3 hemi = mix(groundColor, skyColor, hemiMix);
    vec3 hemiWColor = hemi * color;

    vec3 diffuseColor = color * st;
    vec3 result = pow(diffuseColor + hemiWColor, vec3(1.0 / 2.2));
    finalColor = vec4(result, 1.0);
}
