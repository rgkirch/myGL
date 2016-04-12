#version 330 core

layout (location = 0) in float x;
layout (location = 1) in float y;
uniform float viewOffsetX = 0.0;
uniform float viewOffsetY = 0.0;
uniform float unitsPerPixelX = 1.0;
uniform float unitsPerPixelY = 1.0;
uniform float screenWidth;
uniform float screenHeight;

void main() {
	gl_Position = vec4((x + viewOffsetX) / unitsPerPixelX / (screenWidth / 2.0), (y + viewOffsetY) / unitsPerPixelY / (screenHeight / 2.0), 0.0, 1.0);
}
