#version 330

smooth in vec2 uv;
out vec4 outColour;

uniform sampler2D tex;
uniform vec4 colour;

void main(){
	vec4 texColour = texture(tex,vec2(uv.x,-uv.y));
	outColour.rgb = colour.rgb + texColour.rgb;
	if(colour.a <= texColour.a)
		outColour.a = colour.a;
	else
		outColour.a = texColour.a;
}
