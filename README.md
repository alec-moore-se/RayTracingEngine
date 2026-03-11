# Ray Tracing Engine 
--------------------------
This ray tracing engine will be able to produce complex images and shapes with varying textures, with other features that one would see in a ray tracing engine.

## Core Components On Completion
---------------------------------------
* A camera with configurable position, orientation, and field of view  
* Anti-aliasing  
* Ray/sphere intersections  
* Ray/triangle intersections  
* The ability to load textures (file format(s) of your choice; may use third-party libraries)  
* Textured spheres and triangles  
* The ability to load and render triangle meshes (file format(s) of your choice; may use third-party libraries for loading)  
* A spatial subdivision acceleration structure of your choice  
* Specular, diffuse, and dielectric materials (per the first volume of the Ray Tracing in One Weekend series)  
* Emissive materials (lights)  

## Additional Components
------------------------------------------------
* Volume rendering (smoke, clouds, etc.): 10  
* Quads: 10  
* Motion blur: 10  
* Defocus blur/depth of field: 10  
* Object instancing: 10  
* Perlin noise: 10   
* Importance sampling: 15  
* Parallelization: 10  

### ISU and Author
-----------------------------
Author: Alec Moore (Solo Developed)  
Instructor: Jeremy Sheaffer  
Course: Introduction to Computer Graphics - 3360  

### How to Run
-----------------
Download the repository
There is a Makefile that can make the project on any system capable of running C++ compiler
Then run the command ./raytrace # <-- a number to specify what scene to draw
New scenes may can be coded in the main file or ported to with an added switch case to draw 
