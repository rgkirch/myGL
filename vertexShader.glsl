#version 330 core

layout (location = 0) in float x;
layout (location = 1) in float y;
uniform float cameraOffsetX = 0.0;
uniform float cameraOffsetY = 0.0;
uniform float cameraScaleX = 1.0;
uniform float cameraScaleY = 1.0;

void main() {
	gl_Position = vec4(x * cameraScaleX - cameraOffsetX, y * cameraScaleY - cameraOffsetY, 0.0, 1.0);
}
