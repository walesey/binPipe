binPipe

Binary visualisation tool

Requirements:
  * make
  * gcc
  * opengl/glut

Compile with: `$ make run` or `$ make runMac`

Write data to `./data`: eg. `$ ls -R / >> data`

The purpose of this program is to create a 3D visualization of binary data. 
The way it works is that reads chuncks of data from the `./data` file,
It then takes 3 bytes at a time and uses those 3 values as xyz coordinates.
It continues to scan the data 3 bytes at a time, mapping these coordinate to pixel colors in a volumetric image.
The result is an interesting visualization.

![screenshot](https://raw.githubusercontent.com/walesey/binPipe/master/screenshot.png)