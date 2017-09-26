#include "common.h"

void processKeys(unsigned char key, int x, int y) {
	if (key == 113) {
		exit(0);
	}
}

void compileShader(GLuint shader) {
	glCompileShader(shader);
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		int logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		char *logs = malloc(sizeof(char) * logLength);
		glGetShaderInfoLog(shader, logLength, NULL, logs);

		printf("failed to compile shader: %s", logs);
		free(logs);
	}
}

GLuint loadVBO(geometry_t geometry) {
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, geometry.nbVerticies*4, geometry.verticies, GL_STATIC_DRAW);
	return vbo;
}

GLuint loadIBO(geometry_t geometry) {
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.nbVerticies*4, geometry.indicies, GL_STATIC_DRAW);
	return ibo;
}