#include "renderer2D.h"

#include "renderer3D.h"

GLuint v, f, p;

GLuint quadVbo, quadIbo;
GLsizei nbQuadElements;

GLuint texID;

int winWidth;
int winHeight;


void (*update)();

float quadVerticies[] = {
	0, 0, 0, 0, 0,
	0, 1, 0, 0, 1,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
  0, 1, 0, 0, 1,
	1, 1, 0, 1, 1
};

unsigned int quadIndicies[] = {
	0, 1, 2,
	3, 4, 5
};

void renderer2D_changeSize(int w, int h) {
	if(h == 0)
		h = 1;

	winWidth = w;
	winHeight = h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
}

void renderer2D_renderScene(void) {
	Mat4 ortho, model;
	
	if (update != NULL) update();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// set buffers
	glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIbo);

	// set verticies attribute
	GLuint vertAttrib = glGetAttribLocation(p, "vert");
	glEnableVertexAttribArray(vertAttrib);
	glVertexAttribPointer(vertAttrib, 3, GL_FLOAT, 0, 5*4, (void*)0);

	GLuint uvAttrib = glGetAttribLocation(p, "uv");
	glEnableVertexAttribArray(uvAttrib);
	glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, 0, 5*4, (void*)(3*4));

	Math_Ortho(ortho, 0, winWidth, winHeight, 0);
	GLint orthoLoc = glGetUniformLocation(p, "ortho");
	glUniformMatrix4fv(orthoLoc, 1, 0, &ortho[0]);

	Math_Scale3D(model, winWidth, winHeight, 1.0);
	GLint modelLoc = glGetUniformLocation(p, "model");
	glUniformMatrix4fv(modelLoc, 1, 0, &model[0]);

	glDrawElements(GL_TRIANGLES, nbQuadElements, GL_UNSIGNED_INT, (void*)0);

	glutSwapBuffers();
}

void renderer2D_setShaders() {
	char *vs = NULL,*fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("shaders/texture.vert");
	fs = textFileRead("shaders/texture.frag");

	const char * ff = fs;
	const char * vv = vs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);free(fs);

	compileShader(v);
	compileShader(f);

	p = glCreateProgram();
	glAttachShader(p,f);
	glAttachShader(p,v);

	glLinkProgram(p);
	glUseProgram(p);

	glBindFragDataLocation(p, 0, "outputColor");
}

GLuint loadTexture2D(image_t img, GLuint textureUnit) {
	GLuint texId;
	glGenTextures(1, &texId);
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		img.width,
		img.height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		img.data
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return texId;
}

void renderer2D_setImage(image_t img) {
	glDeleteTextures(1, &texID);

	GLint textureLoc = glGetUniformLocation(p, "tex");
	glUniform1i(textureLoc , 0);
	texID = loadTexture2D(img, GL_TEXTURE0);
}

void initQuad() {
	geometry_t quad;
	quad.indicies = quadIndicies;
	quad.verticies = quadVerticies;
	quad.nbIndicies = sizeof(quadIndicies);
	quad.nbVerticies = sizeof(quadVerticies);

	quadVbo = loadVBO(quad);
	quadIbo = loadIBO(quad);
	nbQuadElements = quad.nbIndicies;
}

void renderer2D_onUpdate(void (*fn)()) {
	update = fn;
}

void renderer2D_initRenderer(void) {
	glutDisplayFunc(renderer2D_renderScene);
	glutIdleFunc(renderer2D_renderScene);
	glutReshapeFunc(renderer2D_changeSize);
	glutKeyboardFunc(processKeys);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.1, 1.0);
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	renderer2D_setShaders();
	initQuad();
}