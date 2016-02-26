#version 330

smooth in vec2 uv;
out vec4 outColour;

uniform sampler2D sdfTexture;
uniform vec4 colour;
uniform float spread;
uniform float scaleFactor;

void main() {
	float smoothing = 0.25 / (spread * scaleFactor);
	float dist = texture(sdfTexture, vec2(uv.x, -uv.y)).r;
	float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, dist)*colour.a;
	outColour = vec4(colour.rgb, alpha);
}
