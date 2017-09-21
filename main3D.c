#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <fcntl.h>
// #include <pthread.h>

#include "renderers/renderer3D.h"
#include "renderers/textfile.h"
#include "binary/binary.h"


#define READ_BUFFER 256
#define IMAGE_WIDTH 32
#define FRAMES_PER_UPDATE 10

int updateCounter = 0;

// pthread_mutex_t lock;

void initImageFile() {
  File f;
  f.size = sizeof(unsigned char)*IMAGE_WIDTH*IMAGE_WIDTH*IMAGE_WIDTH*4;
  f.data = malloc(f.size);
  fileWrite("image", f);
  free(f.data);
}

void readInputData() {
  int filed = open("data", O_RDWR );
  char buf[READ_BUFFER];

  if( !filed ) {
    printf("Openfile error\n");
    exit(-1);
  }

  int nbytes;
  while(1) {
    nbytes = read(filed, buf, READ_BUFFER);
    printf("read %d bytes from file.\n", nbytes);
    if(nbytes > 0) {
      File f = fileRead("image");
      int intensity = 1000/cbrt(nbytes);
      linear3DVisualisation(buf, f.data, nbytes, IMAGE_WIDTH, intensity);
      fileWrite("image", f);
      free(f.data);
    }
    usleep(100*1000);
  }
}

void renderUpdate() {
  updateCounter--;
  if (updateCounter <=0) {
    File f = fileRead("image");
    updateCounter = FRAMES_PER_UPDATE;
    Image img;
    img.data = f.data;
    img.width = IMAGE_WIDTH;
    img.height = IMAGE_WIDTH;
    img.depth = IMAGE_WIDTH;
    setImage(img);
  }
}

void launchWindow(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
  glutCreateWindow("Bin Pipe 3D");
  glutReshapeWindow(800, 800);
  initRenderer();
  onUpdate(renderUpdate);
  glutMainLoop();
}

int main(int argc, char **argv) {
  initImageFile();
  pid_t pid;
  if ((pid = fork()) < 0){
      fprintf(stderr, "Failed to fork()!\n");
      exit(2);
  }
  if (pid == 0) { // child
    readInputData();
  } else { // parent
    launchWindow(argc, argv);
  }
  return 0;
}
