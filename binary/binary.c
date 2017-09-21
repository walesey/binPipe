#include "binary.h"

void linear3DVisualisation(
  unsigned char *input,     // pointer to the input data buffer
  unsigned char *imageData, // pointer to the output rgba image buffer
  int length,               // number of bytes to process from input
  int resolution,           // pixel width for the output 3d image (max 255)
  int intensity             // rate of pixel transformation
) {
  for (int i=0; (i+2)<length; i+=3) {
    int x = (input[i]*resolution)/256;
    int y = (input[i+1]*resolution)/256;
    int z = (input[i+2]*resolution)/256;

    int index = 4*(x + y*resolution + z*resolution*resolution);

    // existing pixel value
    unsigned char r = imageData[index];
    unsigned char g = imageData[index+1];
    unsigned char b = imageData[index+2];
    unsigned char a = imageData[index+3];
    
    // transform the color
    if (a < (255 - 50)) {
      a = a + intensity;
    }
    r = 50;
    g = 255;
    b = 100;

    // write new values
    imageData[index] = r;
    imageData[index+1] = g;
    imageData[index+2] = b;
    imageData[index+3] = a;
  }
}