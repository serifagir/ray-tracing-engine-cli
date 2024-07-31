#include "vector3.h"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

typedef Vector3<float> Vector3f;

class Sphere {
public:
    Vector3f center;
    float radius;
    float radius_sqr;
    Vector3f material_color, emission_color;   // material color and light emission color
    float transparency, reflection;
    Sphere(
            const Vector3f &cent,
            const float &r,
            const Vector3f &mat_col,
            const float &refl = 0,
            const float &transp = 0,
            const Vector3f &emm_c = 0): center(cent), radius(r), radius_sqr(r*r), material_color(mat_col), emission_color(emm_c), transparency(transp), reflection(refl){}

    //compute intersection of a ray and a sphere
    //t0 and t1 pointers of intersection points
    bool intersect(const Vector3f &ray_origin, const Vector3f &ray_direction, float &t0, float &t1) const {
        Vector3f l = center - ray_origin;       // length vector
        float tca = l.dotProd(ray_direction);       // TR: l vektörünün raydirection vektörü üzerine izdüşümü
        if (tca < 0 ) return false;
        float d2 = l.dotProd(l) - tca * tca;        // nearest point
        if (d2 > radius_sqr) return false;      // dot outside the sphere
        float thc = sqrt(radius_sqr - d2);
        t0 = tca - thc;         //EXPLANATION: thc_____________tca_____________thc
        t1 = tca + thc;         //so t0 = tca - thc and t1 = tca + thc

        return true;
    }
};