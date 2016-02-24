#version 330

layout (location = 0) in vec2 coord2d;
smooth out vec2 uv;

uniform vec2 pos;
uniform vec2 scale;
uniform vec2 charScale;
uniform float character;

const float numXChars = 9;
const float numYChars = 10;

void main() {
	vec2 offset = ((coord2d+1.0)/2.0)*charScale;
	uv.x = offset.x + charScale.x*(mod(character,numXChars));
	uv.y = offset.y + 1 - charScale.y*(floor(character/numXChars)+1);
	gl_Position = vec4((coord2d*scale)+pos,0.0,1.0);
}
