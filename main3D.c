#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "renderers/renderer3D.h"

unsigned char data[4000];

void updateImageData() {
  for (int i=0;i<1000;i++) {
    data[i] = 255;
    data[i+1] = 0;
    data[i+2] = 0;
    data[i+3] = 0;
    if (i%23 == 0) {
      data[i+3] = 255;
    }
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
  glutCreateWindow("red 3D lighted cube");
  glutReshapeWindow(800, 800);
  initRenderer();
  updateImageData();
  Image img;
  img.data = &data[0];
  img.width = 10;
  img.height = 10;
  img.depth = 10;
  setImage(img);
  glutMainLoop();
  return 0;
}
