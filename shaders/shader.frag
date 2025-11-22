#version 450


layout(location = 0) out vec4 color;
layout(push_constant) uniform Push {
	mat2 transform;
	vec2 offset;
	vec4 color; 
} push;


void main() 
{
    color = vec4(push.color);
}
