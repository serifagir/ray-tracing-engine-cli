
# CLI Ray Tracing Engine

A simple ray tracing engine that can be controlled via the command line and renders images as output.


## Ray Tracing Algorithm

![lightingnoshadow](https://github.com/user-attachments/assets/6e5c7bc0-b64c-4b72-8f77-bd5936f81008)
![pixelrender](https://github.com/user-attachments/assets/dce2d239-1580-4fad-8e19-5be084b9ea0c)


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
<img width="1071" alt="Screenshot 2024-08-01 at 01 14 56" src="https://github.com/user-attachments/assets/5d2e69be-6a71-4b64-8a7d-4cbb4f39553b">


### Adding Sphere
position, radius, surface color, reflectivity, transparency

```shell
Enter object parameters: (x,y,z),r,(R,G,B),refl,trans
```

### Adding Light Source
position, radius, surface color, reflectivity, transparency, emission color

```shell
Enter object parameters: (x,y,z),r,(R,G,B),refl,trans,emis
```


## Demo
Scene with ground and light.
![scene2](https://github.com/user-attachments/assets/56931500-71b7-42e8-bc03-5f35c76dc362)


Parameters: 

            0,-10004,-20,10000,0,0.7,0,0,0 (ground)
            0,20,-30,3,0,0,0,0,0,3 (light)


Scene with ground, light and spheres
![scene](https://github.com/user-attachments/assets/2bd7a1d7-d8dc-481b-aaf9-7b04eb38ac6b)

Parameters: 

            0,-10004,-20,10000,0,0.7,0,0,0 (ground)
            0,2,-20,4,1,0,0,1,0.5 (sphere 1)
            5,-1,-15,2,0,1,0,1,0 (sphere 2)
            5,0,-25,3,0,0,1,1,0 (sphere 3)
            -5.5,-4,-15,3,0.3,0.9,0.9,1,0 (sphere 4)
            0,20,-30,3,0,0,0,0,0,3 (light)


