#version 330

uniform sampler3D tex;

in vec3 worldVertex;
in vec3 eyeDirection;
in vec2 fragTexCoord;

out vec4 outputColor;

void main() {
	vec4 color = texture(tex, vec3(fragTexCoord, 0));
	outputColor = vec4(1, 0, 0, 1);
}
