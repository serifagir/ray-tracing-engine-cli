
# CLI Ray Tracing Engine

A simple ray tracing engine that can be controlled via the command line and renders images as output.


## Ray Tracing Algorithm




![Logo](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/th5xamgrr6se0x5ro4g6.png)


## Compiling & Running

!! C++20 or newer required. 

Clone the project

```bash
  git clone https://github.com/serifagir/ray-tracing-engine-cli
```

Go to the project directory

```bash
  cd ray-tracing-engine-cli
```

Compile


```bash
  c++ -O3 -o raytracer main.cpp
```
or
```bash
  clang++ -O3 -o raytracer raytracer.cpp
```
Run executable file

```bash
  ./raytracer
```


## Usage/Examples

### Create project


### Adding Sphere
position, radius, surface color, reflectivity, transparency, emission color

```shell
Enter object parameters: (x,y,z),r,(R,G,B),refl,trans
```

### Adding Light Source
```shell
Enter object parameters: (x,y,z),r,(R,G,B),refl,trans,emis
```


## Demo
Scene with ground and light.


Parameters: 

            0,-10004,-20,10000,0,0.7,0,0,0 (ground)
            0,20,-30,3,0,0,0,0,0,3 (light)


Scene with ground, light and spheres

Parameters: 

            0,-10004,-20,10000,0,0.7,0,0,0 (ground)
            0,2,-20,4,1,0,0,1,0.5 (sphere 1)
            5,-1,-15,2,0,1,0,1,0 (sphere 2)
            5,0,-25,3,0,0,1,1,0 (sphere 3)
            -5.5,-4,-15,3,0.3,0.9,0.9,1,0 (sphere 4)
            0,20,-30,3,0,0,0,0,0,3 (light)


