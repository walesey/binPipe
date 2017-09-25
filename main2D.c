#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "renderers/renderer2D.h"

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_4_1_CORE_PROFILE);
  glutCreateWindow("red 3D lighted cube");
  glutReshapeWindow(800, 800);
  renderer3D_init();
  glutMainLoop();
  return 0;
}
