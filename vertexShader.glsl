                              # vertexShader.glsl
#version 330 core

layout (location = 0) in int x;
layout (location = 1) in int y;

void main() {
	gl_Position = vec4( x, y, 0.0, 1.0 );
}
