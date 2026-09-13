#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 myColors;
layout(location = 4) in vec2 UV;

out vec3 o_normal;
out vec3 vColors;
out vec2 o_UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    o_UV = UV;
    vColors = myColors;
    o_normal = normalize(mat3(model) * vertexNormal);
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}
