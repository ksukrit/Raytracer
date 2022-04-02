#ifndef CAMERA_H
#define CAMERA_H
#include "Vector.h"
#include "ray.h"
class Camera{
    Vector origin, lower_left_corner, horizontal, vertical;
    Vector u,w,v;
    double lens_radius;
public:
    Camera(double vfov, double aspect_ratio) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto focal_length = 1.0;

        origin = Vector(0, 0, 0);
        horizontal = Vector(viewport_width, 0.0, 0.0);
        vertical = Vector(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal/2 - vertical/2 - Vector(0, 0, focal_length);
    }

    Camera(Vector lookfrom,
            Vector lookat,
            Vector   vup,
            double vfov, 
            double aspect_ratio){
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = (lookfrom - lookat).normalize();
        u = vup.cross(w).normalize();
        v = w.cross(u);

        origin = lookfrom;
        horizontal = u*viewport_width ;
        vertical = v*viewport_height ;
        lower_left_corner = origin - horizontal/2 - vertical/2 - w;


    }

        Camera(Vector lookfrom,
            Vector lookat,
            Vector   vup,
            double vfov, 
            double aspect_ratio,
            double aperture,
            double focus_dist){
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = (lookfrom - lookat).normalize();
        u = vup.cross(w).normalize();
        v = w.cross(u);

        origin = lookfrom;
        horizontal = u*viewport_width * focus_dist ;
        vertical = v*viewport_height * focus_dist;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

        lens_radius = aperture / 2;
    }

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
        Vector rd = lens_radius * random_in_unit_disk();
        Vector offset = u * horizontal + v * vertical;

        return ray(origin + offset, lower_left_corner + horizontal*u + vertical*v - origin - offset);
    }

};
#endif