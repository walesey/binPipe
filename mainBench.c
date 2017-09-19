#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "renderers/math.h"

int main(int argc, char **argv) {
  Mat4 m = {1, 2, 3,4, 5, 6, 7, 8, 9, 10 , 11, 12, 13,14,15};
  for (int i=0;i<1;i++) {
    Vec3 m2;
    Math_Translate3D(m2, 0.001,0.002,0.003);
    Mat4_Mul(m, m, m2);
  }
  Mat4_Print(m);
  return 0;
}
