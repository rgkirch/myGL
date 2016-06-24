#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 vertexUV;
out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	//gl_Position = translationMatrix * rotationMatrix * scaleMatrix * vec4(position, 0.0, 1.0);
	gl_Position = projection * view * model * vec4(position, 0.0, 1.0);

    UV = vertexUV;
}
