#version 450

layout(location = 0) in vec2 a_Position;

layout(location = 0) out vec3 v_Color;

layout(push_constant, std430) uniform pc {
    mat4 transform;
} u_ModelTransform;

layout(set = 0, binding = 0) uniform CameraUBO {
    mat4 viewProj;
} u_ViewProjection;

void main()
{
    v_Color = vec3(1.0, 0.0, 0.0);
    gl_Position = u_ViewProjection.viewProj * u_ModelTransform.transform * vec4(a_Position, 0.0, 1.0);
}
