#include "renderer3D.h"

GLuint v, f, p;

GLuint cubeVbo, cubeIbo;
GLsizei nbCubeElements;

GLuint texID;

float camAngle = 1.0;
float camHeight = 2.0;
float camZoom = 3.0;
float aspect = 1.0;

float mouse[2] = {0,0};
int rightClickState = 0;

void (*update)();

float cubeVerticies[] = {
	-1, -1, -1,
	-1,  1, -1,
	-1,  1,  1,
	-1,  1,  1,
	-1, -1,  1,
	-1, -1, -1,
	 1, -1, -1,
	 1, -1,  1,
	 1,  1,  1,
	 1,  1,  1,
	 1,  1, -1,
	 1, -1, -1,
	-1, -1, -1,
	-1, -1,  1,
	 1, -1,  1,
	 1, -1,  1,
	 1, -1, -1,
	-1, -1, -1,
	-1, -1,  1,
	-1,  1,  1,
	 1,  1,  1,
	 1,  1,  1,
	 1, -1,  1,
	-1, -1,  1,
	-1,  1,  1,
	-1,  1, -1,
	 1,  1, -1,
	 1,  1, -1,
	 1,  1,  1,
	-1,  1,  1,
	 1, -1, -1,
	 1,  1, -1,
	-1,  1, -1,
	-1,  1, -1,
	-1, -1, -1,
	 1, -1, -1,
};

unsigned int cubeIndicies[] = {
	0,  1,  2,
	3,  4,  5,
	6,  7,  8, 
	9,  10, 11, 
	12, 13, 14,
	15, 16, 17,
	18, 19, 20, 
	21, 22, 23, 
	24, 25, 26, 
	27, 28, 29, 
	30, 31, 32, 
	33, 34, 35, 36,
};

void renderer3D_changeSize(int w, int h) {
	if(h == 0)
		h = 1;

	aspect = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
}

void renderer3D_renderScene(void) {
	Mat4 projection, camera, model;
	Vec3 lookat = {0.0, 0.0, 0.0};
	Vec3 up = {0.0, 1.0, 0.0};
	Vec3 camLoc = {
		sin(camAngle)*camZoom,
		camHeight*camZoom,
		cos(camAngle)*camZoom
	};

	if (update != NULL) update();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// set buffers
	glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIbo);

	// set verticies attribute
	GLuint vertAttrib = glGetAttribLocation(p, "vert");
	glEnableVertexAttribArray(vertAttrib);
	glVertexAttribPointer(vertAttrib, 3, GL_FLOAT, 0, 3*4, (void*)0);

	Math_Perspective(projection, Math_DegToRad(CAMERA_FOV_Y), aspect, CAMERA_NEAR, CAMERA_FAR);
	GLint projectionLoc = glGetUniformLocation(p, "projection");
	glUniformMatrix4fv(projectionLoc, 1, 0, &projection[0]);

	Math_LookAtV(camera, camLoc, lookat, up);
	GLint cameraLoc = glGetUniformLocation(p, "camera");
	glUniformMatrix4fv(cameraLoc, 1, 0, &camera[0]);

	Math_Translate3D(model, 0.0, 0.0, 0.0);
	GLint modelLoc = glGetUniformLocation(p, "model");
	glUniformMatrix4fv(modelLoc, 1, 0, &model[0]);

	GLint cameraTranslationLoc = glGetUniformLocation(p, "cameraTranslation");
	glUniform3f(cameraTranslationLoc, camLoc[0], camLoc[1], camLoc[2]);

	glDrawElements(GL_TRIANGLES, nbCubeElements, GL_UNSIGNED_INT, (void*)0);

	glutSwapBuffers();
}

void mouseClick(int button, int state, int x, int y) {
	mouse[0] = x;
	mouse[1] = y;
	rightClickState = button == 2;
}

void mouseMotion(int x, int y) {
	float delta[2] = {x-mouse[0], y-mouse[1]};
	if (rightClickState) {
		camZoom += CAMERA_SCROLL_SPEED*delta[1];
		if (camZoom < 0.001) camZoom = 0.001;
	} else {
		camAngle -= CAMERA_SPEED*delta[0];
		camHeight += CAMERA_SPEED*delta[1];
	}
	mouse[0] = x;
	mouse[1] = y;
}

void renderer3D_setShaders() {
	char *vs = NULL,*fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("shaders/tracer.vert");
	fs = textFileRead("shaders/tracer.frag");

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

GLuint loadTexture3D(image_t img, GLuint textureUnit) {
	GLuint texId;
	glGenTextures(1, &texId);
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_3D, texId);
	glTexImage3D(
		GL_TEXTURE_3D,
		0,
		GL_RGBA,
		img.width,
		img.height,
		img.depth,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		img.data
	);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return texId;
}

void renderer3D_setImage(image_t img) {
	glDeleteTextures(1, &texID);

	GLint textureLoc = glGetUniformLocation(p, "tex");
	glUniform1i(textureLoc , 0);
	texID = loadTexture3D(img, GL_TEXTURE0);

	GLint marchLoc = glGetUniformLocation(p, "march");
	glUniform1f(marchLoc, 1.0/img.width);
	GLint detailLoc = glGetUniformLocation(p, "detail");
	glUniform1f(detailLoc, img.width/RAY_RESOLUTION);
}

void initCube() {
	geometry_t cube;
	cube.indicies = cubeIndicies;
	cube.verticies = cubeVerticies;
	cube.nbIndicies = sizeof(cubeIndicies);
	cube.nbVerticies = sizeof(cubeVerticies);

	cubeVbo = loadVBO(cube);
	cubeIbo = loadIBO(cube);
	nbCubeElements = cube.nbIndicies;
}

void renderer3D_onUpdate(void (*fn)()) {
	update = fn;
}

void renderer3D_initRenderer(void) {
	glutDisplayFunc(renderer3D_renderScene);
	glutIdleFunc(renderer3D_renderScene);
	glutReshapeFunc(renderer3D_changeSize);
	glutKeyboardFunc(processKeys);
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouseClick);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.1, 1.0);
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	renderer3D_setShaders();
	initCube();
}