#version 450

layout(location = 0) in vec3 a_Position;

layout(location = 0) out vec4 v_Color;

layout(push_constant, std430) uniform pc {
    mat4 transform;
} u_ModelTransform;

layout(set = 0, binding = 0) uniform CameraUBO {
    mat4 viewProj;
} u_ViewProjection;

layout(set = 1, binding = 0) uniform MaterialUBO {
    vec4 color;
} u_Material;

void main() {
    v_Color = u_Material.color;
    gl_Position = u_ViewProjection.viewProj * u_ModelTransform.transform * vec4(a_Position, 1.0);
}
