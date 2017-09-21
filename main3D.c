#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>

#include "renderers/renderer3D.h"
#include "renderers/textfile.h"
#include "binary/binary.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define READ_BUFFER 100000
#define IMAGE_WIDTH 128
#define FRAMES_PER_UPDATE 4
#define FRAMES_PER_FADE 15

int counter = 0;
int updateCounter = 0;

int nbBytes;
unsigned char *imageData;
Image img;

pthread_t read_thread;
pthread_mutex_t lock;

/*
  Allocate buffer memory and shared memory
*/
void initImageData() {
  nbBytes = sizeof(unsigned char)*IMAGE_WIDTH*IMAGE_WIDTH*IMAGE_WIDTH*4;
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_ANONYMOUS | MAP_SHARED;
  imageData = mmap(NULL, nbBytes, protection, visibility, 0, 0);
  img.data = malloc(nbBytes);
  img.width = IMAGE_WIDTH;
  img.height = IMAGE_WIDTH;
  img.depth = IMAGE_WIDTH;
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
void readInputData() {
  int filed = open("data", O_RDWR );
  char buf[READ_BUFFER];

  if (!filed) {
    printf("Openfile error\n");
    exit(-1);
  }

  int nbytes;
  while (1) {
    nbytes = read(filed, buf, READ_BUFFER);
    if(nbytes > 0) {
      pthread_mutex_lock(&lock);
      int intensity = 1000/cbrt(nbytes);
      linear3DVisualisation(buf, imageData, nbytes, IMAGE_WIDTH, intensity);
      pthread_mutex_unlock(&lock);
    }    
    usleep(100*1000);
  }
}

/*
  renderUpdate - function called by the renderer once per frame.
*/
void renderUpdate() {
  usleep(15000);
  updateCounter--;
  if (updateCounter <= 0) {
    updateCounter = FRAMES_PER_UPDATE;
    pthread_mutex_lock(&lock);
    memcpy(img.data, imageData, nbBytes);
    setImage(img);
    pthread_mutex_unlock(&lock);
  }
  fadeImage();
}

/*
  launchWindow - launch a new glut window and fire up the 3D renderer.
*/
void launchWindow(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
  glutCreateWindow("Bin Pipe 3D");
  glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
  initRenderer();
  onUpdate(renderUpdate);
  glutMainLoop();
}


int main(int argc, char **argv) {
  initImageData();

  if (pthread_mutex_init(&lock, NULL) != 0) {
    fprintf(stderr, "mutex init failed\n");
    return 1;
  }

  if (pthread_create(&read_thread, NULL, readInputData, NULL)) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
  }

  launchWindow(argc, argv);

  pthread_mutex_destroy(&lock);
  return 0;
}
