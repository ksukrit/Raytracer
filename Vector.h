#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include "utility.h"
class Vector {
    double x, y, z;
public:
    Vector(){
        x = 0;
        y = 0;
        z = 0;
    }

    Vector(double x, double y, double z):x(x), y(y), z(z){}

    Vector(const Vector& v):x(v.x), y(v.y), z(v.z){}

    Vector operator+(const Vector& v) const{
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector operator-(const Vector& v) const {
        return Vector(x - v.x, y - v.y, z - v.z);
    }

    Vector operator-() const {
        return Vector(-x, -y, -z);
    }

    Vector operator*(const Vector& v) const {
        return Vector(x * v.x, y * v.y, z * v.z);
    }

    Vector operator/(const double& d) const {
        return Vector(x / d, y / d, z / d);
    }

    Vector multiply(const Vector& v) const {
        return Vector(x * v.x, y * v.y, z * v.z);
    }
    
    Vector operator+= (const Vector& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }


    double dot(const Vector& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    double length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector operator%(const Vector& v) const {
        return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    Vector cross(const Vector& v) const {
        return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    
    static Vector randomVector(){
        return Vector(random_double(), random_double(), random_double());
    }

    static Vector randomVector(double min, double max){
        return Vector(random_double(min, max), random_double(min, max), random_double(min, max));
    }



    double x_component() const {
        return x;
    }

    double y_component() const {
        return y;
    }

    double z_component() const {
        return z;
    }

    inline static Vector random() {
        return Vector(random_double(), random_double(), random_double());
    }

    inline static Vector random(double min, double max) {
        return Vector(random_double(min,max), random_double(min,max), random_double(min,max));
    }


    Vector normalize()    {
        double l = length();
        if (l == 0)
            return Vector(0, 0, 0);
        else {
            return Vector(x / l, y / l, z / l);
        }
    }
};


double dot(const Vector &u, const Vector &v) {
    return u.x_component() * v.x_component()
         + u.y_component() * v.y_component()
         + u.z_component() * v.z_component();
}

inline Vector random_in_unit_disk() {
    while (true) {
        auto p = Vector(random_double(-1,1), random_double(-1,1), 0);
        if (p.length() >= 1) continue;
        return p;
    }
}

// Both these functions are needed so that we can multiply double * vector and vector * double
inline Vector operator*(const double& d, const Vector& v) {
    return Vector(v.x_component() * d, v.y_component() * d, v.z_component() * d);
}

inline Vector operator*(const Vector &v, double t) {
    return t * v;
}

Vector randomPoint() {
    while(true){
        auto v = Vector::randomVector(-1,1);
        if(v.length() > 1) continue;
        return v.normalize();
    }
}
#endif