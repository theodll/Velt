#version 450

layout(location = 1) in vec2 v_UV;

layout(location = 0) out vec4 outColor;


layout(set = 1, binding = 0) uniform MaterialUBO {
    vec4 color;
} u_Material;

void main()
{
   outColor = u_Material.color;
   // outColor = vec4(v_UV, 0.0, 1.0);
}
