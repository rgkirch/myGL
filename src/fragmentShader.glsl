#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D texture;

void main() {
	color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//color = texture2D(texture, UV);
}
