# Buggy-Project

## Image processing
We will use clmg as the image processing library as its small and lightweight. All you need it the clmg.h file.
http://cimg.eu/

## Including headers in visual studio project
Project -> Properties -> C/C++ -> General -> Additional include directories

Then include the directory that contains your header/source files. This will allow you to reference them from other classes

## Sensor plan for finding orientation and position
- Use 2 infrared sensors on either side of a camera

### Finding the angle of intersection between the two adjacent white/black cells:
https://uk.mathworks.com/help/images/examples/measuring-angle-of-intersection.html
- Threshold the image to a black/white scale
- Perform a bwtraceboundary routine

### The sensors will identify the color of the cell directly below it
https://www.adafruit.com/product/1334
- The infrared sensors can detect the color of the cells
