#version 330

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;
uniform vec3 cameraTranslation;

in vec3 vert;

out vec3 worldVertex;
out vec3 worldCam;

void main() {
	worldVertex = (model * vec4(vert,1)).xyz;
	worldCam = cameraTranslation;

	gl_Position = projection * camera * model * vec4(vert, 1);
}
