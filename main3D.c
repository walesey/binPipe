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


#define READ_BUFFER 100000
#define IMAGE_WIDTH 128
#define FRAMES_PER_UPDATE 10

int updateCounter = 0;
int running = 1;

int nbBytes;
unsigned char *imageData;
Image img;

pid_t pid;
pthread_mutex_t lock;

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

void readInputData() {
  int filed = open("data", O_RDWR );
  char buf[READ_BUFFER];

  if (!filed) {
    printf("Openfile error\n");
    exit(-1);
  }

  int nbytes;
  while (running) {
    nbytes = read(filed, buf, READ_BUFFER);
    printf("read %d bytes from file.\n", nbytes);
    if(nbytes > 0) {
      pthread_mutex_lock(&lock);
      int intensity = 1000/cbrt(nbytes);
      linear3DVisualisation(buf, imageData, nbytes, IMAGE_WIDTH, intensity);
      pthread_mutex_unlock(&lock);
    }
    usleep(100*1000);
  }
}

void renderUpdate() {
  usleep(15000);
  updateCounter--;
  if (updateCounter <=0) {
    updateCounter = FRAMES_PER_UPDATE;
    pthread_mutex_lock(&lock);
    memcpy(img.data, imageData, nbBytes);
    setImage(img);
    pthread_mutex_unlock(&lock);
  }
}

void launchWindow(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
  glutCreateWindow("Bin Pipe 3D");
  glutReshapeWindow(400, 400);
  initRenderer();
  onUpdate(renderUpdate);
  glutMainLoop();
}

int main(int argc, char **argv) {
  initImageData();

  if (pthread_mutex_init(&lock, NULL) != 0) {
    printf("\n mutex init failed\n");
    return 1;
  }

  if ((pid = fork()) < 0){
    fprintf(stderr, "Failed to fork()!\n");
    return 1;
  }

  if (pid == 0) { // child
    readInputData();
  } else { // parent
    launchWindow(argc, argv);
  }

  pthread_mutex_destroy(&lock);
  running = 0;
  return 0;
}
