#version 330 core

in vec2 UV;
out vec3 color;

uniform sampler2D texture;

void main() {
	//color = vec3(1.0f, 0.0f, 0.0f);
	//color = vec3(UV, 0.0f);
	color = texture2D(texture, UV).rgb;
}
