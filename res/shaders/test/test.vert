#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

layout(binding = 0) uniform VUniformBuffer { mat4  u_ProjectionViewMatrix; } ubuffer;

layout(location = 0) out vec4 v_Color;

void main()
{
    gl_Position = ubuffer.u_ProjectionViewMatrix * vec4(a_Position, 1.0);
    v_Color = a_Color;
}