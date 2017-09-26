#version 330

uniform mat4 ortho;
uniform mat4 model;

in vec3 vert;
in vec2 uv;

out vec2 fragUV;

void main() {
	fragUV = uv;
	gl_Position = ortho * model * vec4(vert, 1);
}
