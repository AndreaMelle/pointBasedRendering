#version 150 core

in vec3 position;
in vec3 color;
in vec2 texcoord;

out vec3 Color;
out vec2 Texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 overrideColor;

void main() {
	Color = overrideColor * color;
	Texcoord = texcoord;
	gl_Position = proj * view * model * vec4(position, 1.0);
}