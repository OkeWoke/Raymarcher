# Raymarcher
This is a CPU based Raymarching renderer that I wrote in one day.
It does not use any external libraries except for OpenCV to simply display and save images.

Currently it only supports planes, spheres and the Mandlebulb as scene objects.
It is multithreaded pixel wise.

## Todo:
 - Define the scene and rendering settings via a config file that is read during run time.
 - More Tonemapping methods.
 - Formalise and implement more shading methods  (phong etc)
 - Formalisation of affine transformations via 4x4 matrices. (especially for camera)
 - Acceleration Structures - BVH/Octree
 - Calculation optimisation
 - Animation or Realtime Interaction?
 - Unit tests 
 - More primitive types
 - Possibly merge with the raytracer code
