#version 330

in vec3 o_normal;

out vec4 finalColor;

float saturation(float v)
{
    return clamp(v, 0.0, 1.0);
}

vec3 saturation(vec3 v)
{
    return clamp(v, 0.0, 1.0);
}

void main()
{
    vec3 dir = normalize(vec3(0.5, 0.5, 0));
    float diffFactor = dot(o_normal, dir);
    float st = saturation(diffFactor);
    vec3 color = vec3(0.9, 0.6, 0.3);
    vec3 ambient = color * 0.25;
    vec3 diffuseColor = color * st;
    vec3 result = ambient + diffuseColor;
    finalColor = vec4(result, 1.0);
}
