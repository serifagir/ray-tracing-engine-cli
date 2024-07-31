
# CLI Ray Tracing Engine

A simple ray tracing engine that can be controlled via the command line and renders images as output.


## Ray Tracing Algorithm

![lightingnoshadow](https://github.com/user-attachments/assets/6e5c7bc0-b64c-4b72-8f77-bd5936f81008)
![pixelrender](https://github.com/user-attachments/assets/dce2d239-1580-4fad-8e19-5be084b9ea0c)


### Recursive photon function

Simulation of photos. Fresnel effects used in computing the ratio of reflection and refraction.

https://en.wikipedia.org/wiki/Fresnel_equations

```cpp
Vector3f photon( const Vector3f &ray_origin, const Vector3f &ray_direction, const std::vector<Sphere> &spheres, const int &depth) {
    float nearest_intersection = INFINITY;
    const Sphere* sphere = nullptr;         // kesişen en yakın küre.

    for(unsigned i = 0; i < spheres.size(); i++) {
        float t0 = INFINITY, t1= INFINITY;
        if (spheres[i].intersect(ray_origin, ray_direction, t0, t1)) {
            if (t0 < 0) t0 = t1;
            if (t0 < nearest_intersection) {
                nearest_intersection = t0;
                sphere = &spheres[i];
            }
        }
    }
    if (!sphere) return Vector3f(2);        //if no intersection, return black or bg color.

    Vector3f materialColor = 0;
    Vector3f hit_point = ray_origin + ray_direction * nearest_intersection;         //point of intersection.
    Vector3f normal = hit_point - sphere->center;       //normal at intersection point. Orthogonal to surface.
    normal.normalize();

    float bias = 1e-4;      //TODO: not sure, change to 1e-5 or 1e-6 respectively.
    bool inside = false;

    if(ray_direction.dotProd(normal) > 0) {
        normal = -normal;
        inside = true;
    }

    // REFLECTION AND REFRACTION -> FRESNEL FORMULA
    if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH) { // both transparent and mirrorlike materials like glass
        float ratio = -ray_direction.dotProd(normal);
        float fresnel_effect = l_interp(pow(1 - ratio, 3), 1, 0.1);
        Vector3f refldir = ray_direction - normal * 2 * ray_direction.dotProd(normal);
        refldir.normalize();
        Vector3f reflection = photon(hit_point + normal * bias, refldir, spheres, depth + 1);
        Vector3f refraction = 0;

        if(sphere->transparency) {
            float ior = 1.1, eta = (inside) ? ior : 1/ior;
            float cosi = -normal.dotProd(ray_direction);
            float  k = 1 - eta * eta * (1- cosi * cosi);
            Vector3f refrdir = ray_direction * eta + normal * (eta * cosi - sqrt(k));
            refrdir.normalize();
            refraction = photon(hit_point - normal * bias, refrdir, spheres, depth + 1);
        }
        materialColor = (
                reflection * fresnel_effect + refraction * (1 - fresnel_effect) * sphere-> transparency) * sphere->material_color;
    } else { //diffuse materials
        for (unsigned i = 0; i < spheres.size(); i++) {
            if (spheres[i].emission_color.x > 0) {
                Vector3f  transmission = 1;
                Vector3f light_direction = spheres[i].center - normal;
                light_direction.normalize();
                for (unsigned j = 0; j < spheres.size(); ++j) {
                    if (i != j) {
                        float t0, t1;
                        if (spheres[j].intersect(hit_point + normal * bias, light_direction,t0, t1)) {
                            transmission = 0;
                            break;
                        }
                    }
                }
                materialColor += sphere->material_color * transmission * std::max(float(0), normal.dotProd(light_direction)) * spheres[i].emission_color;
            }
        }
    }
    return materialColor + sphere->emission_color;
}
```



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
### Scene with ground and light.


![scene2](https://github.com/user-attachments/assets/56931500-71b7-42e8-bc03-5f35c76dc362)


Parameters: 

            0,-10004,-20,10000,0,0.7,0,0,0 (ground)
            0,20,-30,3,0,0,0,0,0,3 (light)


### Scene with ground, light and spheres

![scene](https://github.com/user-attachments/assets/2bd7a1d7-d8dc-481b-aaf9-7b04eb38ac6b)

Parameters: 

            0,-10004,-20,10000,0,0.7,0,0,0 (ground)
            0,2,-20,4,1,0,0,1,0.5 (sphere 1)
            5,-1,-15,2,0,1,0,1,0 (sphere 2)
            5,0,-25,3,0,0,1,1,0 (sphere 3)
            -5.5,-4,-15,3,0.3,0.9,0.9,1,0 (sphere 4)
            0,20,-30,3,0,0,0,0,0,3 (light)


#### Built by serifagir
https://serifagir.com
