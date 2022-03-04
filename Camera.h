#ifndef CAMERA_H
#define CAMERA_H
#include "Vector.h"
#include "ray.h"
class Camera{
    Vector origin, lower_left_corner, horizontal, vertical;
public:
    Camera(){
        const auto aspect_ratio = 16.0 / 9.0;
        const int image_width = 400;
        const int image_height = static_cast<int>(image_width / aspect_ratio);

        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin = Vector(0, 0, 0);
        horizontal = Vector(viewport_width, 0, 0);
        vertical = Vector(0, viewport_height, 0);
        lower_left_corner = origin - horizontal/2 - vertical/2 - Vector(0, 0, focal_length);
    }

    ray get_ray(double u, double v) const {
        return ray(origin, lower_left_corner + horizontal*u + vertical*v - origin);
    }

};
#endif