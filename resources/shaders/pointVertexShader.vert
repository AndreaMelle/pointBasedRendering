#version 150 core

in vec3 position;
in vec4 color;
in vec3 u;
in vec3 v;
in vec3 n;

out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float splatMult;
uniform float radius;

void main() {
	//float radius = sqrt(max(dot(u, u), dot(v, v)));
	vec4 posEye = view * model * vec4(position, 1.0);
	
	gl_Position = proj * posEye;
	gl_PointSize = splatMult * radius / posEye[2];
	Color = color;
}