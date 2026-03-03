#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec3 vertexNormal;

out vec3 o_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    o_normal = normalize(vertexNormal);
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}
