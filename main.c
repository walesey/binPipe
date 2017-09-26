#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>

#include "renderers/renderer2D.h"
#include "renderers/renderer3D.h"
#include "renderers/textfile.h"
#include "binary/binary.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define IMAGE_WIDTH_3D 128
#define IMAGE_WIDTH_2D 256

#define FRAMES_PER_UPDATE 4
#define FRAMES_PER_FADE 5

int counter = 0;
int updateCounter = 0;

int nbBytes;
unsigned char *imageData;
image_t img;

pthread_t read_thread;
pthread_mutex_t lock;

/*
  Allocate buffer memory and shared memory
*/
void initImageData(int imageWidth, int dimensions) {
  nbBytes = sizeof(unsigned char)*pow(imageWidth, dimensions)*4;
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_ANONYMOUS | MAP_SHARED;
  imageData = mmap(NULL, nbBytes, protection, visibility, 0, 0);
  img.data = malloc(nbBytes);
  img.width = imageWidth;
  img.height = imageWidth;
  img.depth = imageWidth;
}

/*
  fadeImage - Call once per frame update
  Gradually reduce the alpha of the overall image.
*/
void fadeImage() {
  // throttle the fade
  counter++;
  if (counter<FRAMES_PER_FADE) {
    return;
  }
  counter = 0;
  pthread_mutex_lock(&lock);
  for (int i=0; i<nbBytes; i+=4){
    unsigned char r, g, b, a;
    r = imageData[i]; g = imageData[i+1]; b = imageData[i+2]; a = imageData[i+3];
    if (r < 255) r++;
    if (g > 0) g--;
    if (b > 0) b--;
    if (a > 0) a--;
    imageData[i] = r; imageData[i+1] = g; imageData[i+2] = b; imageData[i+3] = a;
  }
  pthread_mutex_unlock(&lock);
}

/*
  readInputData - open the 'data' file and periodically check for new data.
  data is transformed and written to the imageData buffer.
*/
void readInputData(int dimensions, int imageWidth) {
  while (1) {
    File f = fileRead("data");
    pthread_mutex_lock(&lock);
    int intensity = 1000/cbrt(f.size);
    linearVisualisation(f.data, imageData, f.size, imageWidth, intensity, dimensions);
    pthread_mutex_unlock(&lock);
    if (f.size > 0) free(f.data);
    textFileWrite("data", "");
    usleep(100*1000);
  }
}

void readInputData3D() {
  readInputData(3, IMAGE_WIDTH_3D);
}

void readInputData2D() {
  readInputData(2, IMAGE_WIDTH_2D);
}

/*
  renderUpdate3D - function called by the 3D renderer once per frame.
*/
void renderUpdate(int dimensions) {
  usleep(15000);
  updateCounter--;
  if (updateCounter <= 0) {
    updateCounter = FRAMES_PER_UPDATE;
    pthread_mutex_lock(&lock);
    memcpy(img.data, imageData, nbBytes);
    if (dimensions == 3) {
      renderer3D_setImage(img);
    } else if (dimensions == 2) {
      renderer2D_setImage(img);
    }
    pthread_mutex_unlock(&lock);
  }
  fadeImage();
}

void renderUpdate3D() {
  renderUpdate(3);
}

void renderUpdate2D() {
  renderUpdate(2);
}

/*
  launchWindow - launch a new glut window and fire up the renderer.
*/
void launchWindow(int dimensions) {
#ifdef __APPLE__
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
#else
  glutInitContextVersion (3, 2);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
#endif  
  glutCreateWindow("BinPipe 3D");
  glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
  if (dimensions == 3) {
    printf("launching 3D Renderer\n");
    renderer3D_initRenderer();
    renderer3D_onUpdate(renderUpdate3D);
  } else if (dimensions == 2) {
    printf("launching 2D Renderer\n");
    renderer2D_initRenderer();
    renderer2D_onUpdate(renderUpdate2D);
  }
  glutMainLoop();
}

int main(int argc, char **argv) {
  void *threadFunc;
  int dimensions;
  int imageWidth;
  if (argc == 2 && strcmp(argv[1], "2D") == 0) {
    dimensions = 2;
    threadFunc = readInputData2D;
    imageWidth = IMAGE_WIDTH_2D;
  } else {
    dimensions = 3;
    threadFunc = readInputData3D;
    imageWidth = IMAGE_WIDTH_3D;
  }

  initImageData(imageWidth, dimensions);

  if (pthread_mutex_init(&lock, NULL) != 0) {
    fprintf(stderr, "mutex init failed\n");
    return 1;
  }

  if (pthread_create(&read_thread, NULL, threadFunc, NULL)) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
  }

  glutInit(&argc, argv);
  launchWindow(dimensions);

  pthread_mutex_destroy(&lock);
  return 0;
}
