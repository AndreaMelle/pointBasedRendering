#version 150 core

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texFramebuffer;

void main() {
   	//outColor = texture(texFramebuffer, Texcoord);
	vec4 s1 = texture(texFramebuffer, Texcoord - 1.0 / 300.0 - 1.0 / 200.0);
	vec4 s2 = texture(texFramebuffer, Texcoord + 1.0 / 300.0 - 1.0 / 200.0);
	vec4 s3 = texture(texFramebuffer, Texcoord - 1.0 / 300.0 + 1.0 / 200.0);
	vec4 s4 = texture(texFramebuffer, Texcoord + 1.0 / 300.0 + 1.0 / 200.0);
	vec4 sx = 4.0 * ((s4 + s3) - (s2 + s1));
	vec4 sy = 4.0 * ((s2 + s4) - (s1 + s3));
	vec4 sobel = sqrt(sx * sx + sy * sy);
	outColor = sobel;
}