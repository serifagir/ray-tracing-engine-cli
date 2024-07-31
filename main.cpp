#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include "headers/sphere.h"
#include <string>
#include <sstream>
#include <format>

#define MAX_RAY_DEPTH 5

//Linear interpolation => b * mix + a * (1-mix)
//if mix = 0=> return a;
//if mix = 1=> return b;

float l_interp(const float &a, const float &b, const float &mix) { return b * mix + a * (1 - mix); }

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

void render(const std::vector<Sphere> &spheres, const std::string filename) {
    unsigned width = 640;
    unsigned height = 480;

    Vector3f *image = new Vector3f[width*height], *pixel = image;
    float inv_width = 1 / float(width);
    float inv_height = 1 / float(height);

    float fov = 30;
    float aspect_ratio = width / float(height);
    float angle = tan(M_PI * 0.5 * fov / 180.);

    for(unsigned y = 0; y < height; y++) {
        for(unsigned x = 0; x < width; ++x, ++pixel) {
            float xx = (2 * ((x + 0.5) * inv_width) - 1) * angle * aspect_ratio;
            float yy = (1 - 2 * ((y + 0.5) * inv_height)) * angle;
            Vector3f raydir(xx, yy, -1);
            raydir.normalize();
            *pixel = photon(Vector3f(0), raydir, spheres, 0);
        }
    }
    std::ofstream ofs(std::format("./{}.ppm", filename), std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for(unsigned i = 0; i < width * height; i++) {
        ofs << (unsigned char)(std::min(float(1), image[i].x) * 255) <<
                (unsigned char)(std::min(float(1), image[i].y) * 255) <<
                (unsigned char)(std::min(float(1), image[i].z) * 255);
    }
    ofs.close();
    delete [] image;
}

std::vector<float> parseStringToFloatVector(const std::string& str) {
    std::vector<float> floatVector;
    std::stringstream ss(str);
    std::string temp;

    while (std::getline(ss, temp, ',')) {
        try {
            if (!temp.empty()) {
                floatVector.push_back(std::stof(temp));
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid input: " << temp << " could not be converted to float." << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Input out of range: " << temp << std::endl;
        }
    }

    return floatVector;
}

int main(int argc, char *argv[]) {
    std::cout << "CLI - Raytracing Engine" << std::endl;
    std::cout << "To add sphere: position, radius, material color, reflectivity, transparency" << std::endl;
    std::cout << "To add light source: position, radius, material color, reflectivity, transparency, emission color" << std::endl;
    std::cout << "To render scene, type 'render'" << std::endl;
    std::cout << "To exit, type 'exit'" << std::endl;

    std::string buffer = "";
    std::vector<std::vector<float> > object_params;
    std::string filename = "untitled";

    std::cout << "Project name: ";
    std::cin >> filename;

    while (buffer != "render" && buffer != "exit") {
        std::cout << "Enter object parameters: ";
        std::cin >> buffer;
        if(buffer != "render") {
            object_params.push_back(parseStringToFloatVector(buffer));
        }
    }

    srand48(13);
    std::vector<Sphere> spheres;

    for (unsigned i = 0; i < object_params.size(); i++) {
        spheres.push_back(Sphere(Vector3f(object_params[i][0], object_params[i][1],object_params[i][2]), object_params[i][3], Vector3f(object_params[i][4], object_params[i][5], object_params[i][6]), object_params[i][7], object_params[i][8]));
    }
    
    render(spheres, filename);

    return 0;
}
