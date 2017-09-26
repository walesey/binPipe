binPipe

Binary visualisation tool

Requirements:
  * make
  * gcc
  * opengl/glut

Compile and Run with: `$ make run` or `$ make runMac`
For 2D mode use: `$ make run2D` or `$ make runMac2D`

Write data to `./data`: eg. `$ cat /dev/urandom > data`

The purpose of this program is to create a 3D visualization of binary data. 
The way it works is that reads chuncks of data from the `./data` file,
It then takes 3 bytes at a time and uses those 3 values as xyz coordinates.
It continues to scan the data 3 bytes at a time, mapping these coordinate to pixel colors in a volumetric image.
The result is an interesting visualization.

<img src="https://raw.githubusercontent.com/walesey/binPipe/master/screenshot.png" width="400">
<img src="https://raw.githubusercontent.com/walesey/binPipe/master/screenshot2.png" width="400">