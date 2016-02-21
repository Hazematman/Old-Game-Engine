#version 130

layout (location = 0) in vec2 coord2d;
smooth out vec2 uv;

uniform vec2 pos;
uniform vec2 scale;

void main(){
	uv = (coord2d+1.0)/2;
	gl_Position = vec4((coord2d*scale)+pos,0.0,1.0);
}
