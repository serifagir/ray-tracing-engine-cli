#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <iostream>
#include <cassert>


template<typename T>

class Vector3 {
public:
    T x, y, z;
    Vector3(): x(T(0)), y(T(0)), z(T(0)) {}
    Vector3(T xx): x(xx), y(xx), z(xx) {}
    Vector3(T xx, T yy, T zz): x(xx), y(yy), z(zz) {}
    Vector3& normalize() {
        T nor2 = length_sqr();
        if(nor2 > 0) {
            T invNor = 1 / sqrt(nor2);
            x *= invNor, y *= invNor, z *= invNor;
        }
        return *this;
    }

    Vector3<T> operator * (const T &num) const { return Vector3<T>(x * num, y * num, z * num); }
    Vector3<T> operator * (const Vector3<T> &vector3) const { return Vector3<T>(x * vector3.x, y * vector3.y, z * vector3.z); }
    T dotProd(const Vector3<T> &vector3) const { return x * vector3.x + y * vector3.y + z*vector3.z; }
    Vector3<T> operator - (const Vector3<T> &vector3) const { return Vector3<T>(x - vector3.x, y - vector3.y, z - vector3.z); }
    Vector3<T> operator + (const Vector3<T> &vector3) const { return Vector3<T>(x + vector3.x, y + vector3.y, z + vector3.z); }
    Vector3<T>& operator += (const Vector3<T> &vector3)  { x += vector3.x, y += vector3.y, z += vector3.z; return *this;}
    Vector3<T>& operator *= (const Vector3<T> &vector3)  { x *= vector3.x, y *= vector3.y, z *= vector3.z; return *this;}
    Vector3<T> operator - () const { return Vector3<T>(-x, -y, -z);}
    T length_sqr() const { return x * x + y * y + z * z; }
    T length() const { return sqrt(length_sqr()); }
    friend std::ostream  & operator << (std::ostream &os, const Vector3<T> &vector3) {
        os << "[" << vector3.x << " " << vector3.y << " " << vector3.z << "]";
        return os;
    }
};
