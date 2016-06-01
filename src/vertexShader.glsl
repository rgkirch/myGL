#version 330 core

layout (location = 0) in vec2 position;
//layout (location = 1) in vec2 vertexUV;
out vec2 UV;

uniform mat4 scaleMatrix;
uniform mat4 rotationMatrix;
uniform mat4 translationMatrix;

//uniform float viewOffsetX = 0.0;
//uniform float viewOffsetY = 0.0;
//uniform float unitsPerPixelX = 1.0;
//uniform float unitsPerPixelY = 1.0;
//uniform float screenWidth;
//uniform float screenHeight;

void main() {
	//gl_Position = vec4((position.x + viewOffsetX) / unitsPerPixelX / (screenWidth / 2.0), (position.y + viewOffsetY) / unitsPerPixelY / (screenHeight / 2.0), 0.0, 1.0);
	//gl_Position = vec4(position, 0.0, 1.0);
	gl_Position = translationMatrix * rotationMatrix * scaleMatrix * vec4(position, 0.0, 1.0);

    //UV = vertexUV;
    UV = position;
}
