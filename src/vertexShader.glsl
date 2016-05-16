#version 330 core

layout (location = 0) in vec2 position;
//layout (location = 1) in vec2 vertTexCo;
//out vec2 fragTexCo;

uniform float viewOffsetX = 0.0;
uniform float viewOffsetY = 0.0;
uniform float unitsPerPixelX = 1.0;
uniform float unitsPerPixelY = 1.0;
uniform float screenWidth;
uniform float screenHeight;

void main() {
	gl_Position = vec4((position.x + viewOffsetX) / unitsPerPixelX / (screenWidth / 2.0), (position.y + viewOffsetY) / unitsPerPixelY / (screenHeight / 2.0), 0.0, 1.0);
    //fragTexCo = vertTexCo;
}