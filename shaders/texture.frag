#version 330

uniform sampler2D tex;

in vec2 fragUV;

out vec4 outputColor;

void main() {
	outputColor = texture(tex, fragUV);
}
