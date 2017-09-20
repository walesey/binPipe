#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include "renderers/renderer3D.h"
#include "renderers/textfile.h"
#include "binary/binary.h"


#define IMAGE_WIDTH 64

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
  glutCreateWindow("red 3D lighted cube");
  glutReshapeWindow(800, 800);
  initRenderer();

  unsigned char *imageData = malloc(sizeof(unsigned char)*IMAGE_WIDTH*IMAGE_WIDTH*IMAGE_WIDTH*4);
  File f = fileRead("/Users/joshuawales/Desktop/pics/download.jpeg");
  printf("file size: %d\n", f.size);
  int intensity = 800/cbrt(f.size);
  printf("intensity: %d\n", intensity);
  linear3DVisualisation(&f.data[0], &imageData[0], f.size, IMAGE_WIDTH, intensity);

  Image img;
  img.data = &imageData[0];
  img.width = IMAGE_WIDTH;
  img.height = IMAGE_WIDTH;
  img.depth = IMAGE_WIDTH;
  setImage(img);

  glutMainLoop();
  return 0;
}
