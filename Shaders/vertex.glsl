#version 450

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec3 a_Color;

layout(location = 0) out vec3 v_Color;

layout(set = 0, binding = 0) uniform CameraUBO {
    mat4 viewProj;
} ubo;

void main()
{
    v_Color = a_Color;
    gl_Position = ubo.viewProj * vec4(a_Position, 0.0, 1.0);
}
