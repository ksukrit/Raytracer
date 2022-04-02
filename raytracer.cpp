#include<iostream>
#include<fstream>
#include<vector>
#include "utility.h"
#include "Vector.h"
#include "ray.h"

#include "Object.h"
#include "ObjectList.h"

#include "Material.h"

#include "Sphere.h"

#include "Camera.h"
#include<time.h>
// Using the same class for color and point ideally should have separate classes
using color = Vector;
using point3 = Vector;


/*
    Fix the following:
    Add dielectric medium
*/

void write_color(std::ostream &out, color pixel_color, int spp) {
    auto r = pixel_color.x_component();
    auto g = pixel_color.y_component();
    auto b = pixel_color.z_component();

    auto scale = 1.0 / spp;
    // sqrt for the gamma normalization 
    r = sqrt(scale*r);
    g = sqrt(scale*g);
    b = sqrt(scale*b);
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r,0.0,0.999)) << ' '
        << static_cast<int>(256 * clamp(g,0.0,0.999)) << ' '
        << static_cast<int>(256 * clamp(b,0.0,0.999)) << '\n';
}

color ray_color(ray r,Sphere s) {
    hit_record rec;
    auto t = s.hit(r, 0, std::numeric_limits<double>::max(), rec);
    
    if (t){
        Vector N = (r.at(t) - Vector(0,0,-1)).normalize();
        return color(N.x_component()+1, N.y_component()+1, N.z_component()+1)*0.5;
    }
    Vector unit_direction = r.getDirection().normalize();
    t = 0.5*(unit_direction.y_component() + 1.0);
    return color(1.0, 1.0, 1.0)*(1.0-t) + color(0.5, 0.7, 1.0)*t;
}

// Recursive ray tracing
color ray_color(ray r, ObjectList o,int depth) {
    hit_record rec;

    if(depth <= 0){
        return color(0,0,0);    
    }

    if (o.hit(r, 0.0001, std::numeric_limits<double>::infinity(), rec)){
        ray scattered;
        color attenuation;
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            return ray_color(scattered, o, depth-1)*attenuation;
        }
        //Vector target = rec.p + rec.normal + randomPoint();
        //return ray_color(ray(rec.p, target - rec.p), o, depth-1);
        return color(0,0,0);
    }
    Vector unit_direction = r.getDirection().normalize();
    auto t = 0.5*(unit_direction.y_component() + 1.0);
    return color(1.0, 1.0, 1.0)*(1.0-t) + color(0.5, 0.7, 1.0)*t;
}



ObjectList random_scene() {
    ObjectList world;

    auto ground_material = make_shared<Diffusion>(color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(1000,point3(0,-1000,0), ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Metal>(albedo);
                    world.add(make_shared<Sphere>(0.2,center, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(0.2,center, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Diffusion>(color(0.4, 0.2, 0.1));
                    world.add(make_shared<Sphere>(0.2,center, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Diffusion>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(1.0,point3(0, 1, 0), material1));

    auto material2 = make_shared<Diffusion>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(1.0,point3(-4, 1, 0), material2));

    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(1.0,point3(4, 1, 0), material3));

    return world;
}


int main(){
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int spp = 500;
    const int maxDepth = 50;

    clock_t start, end;


    auto material_ground = make_shared<Diffusion>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Diffusion>(color(0.7, 0.3, 0.3));
    auto material_left   = make_shared<Metal>(color(0.8, 0.8, 0.8),0.3);
    auto material_right  = make_shared<Metal>(color(0.8, 0.6, 0.2),1.0);

    Vector lookfrom(13,2,3);
    Vector lookat(0,0,0);
    Vector vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);


    //Camera cam(120.0,aspect_ratio); // change the fov to change the image size
    //Camera cam(point3(-2,2,1), point3(0,0,-1), point3(0,1,0), 20, aspect_ratio);
    std::ofstream op_file;
    // write output as a ppm file
    op_file.open("output.ppm");
    //Sphere s(0.5,Vector(0,0,-1),ReflectionType::Refr);
    ObjectList objList;

    objList.add(make_shared<Sphere>(0.5,Vector(0,0,-1),material_center));
    objList.add(make_shared<Sphere>(0.5,Vector(1,0,-1),material_right));
    objList.add(make_shared<Sphere>(0.5,Vector(-1,0,-1),material_left));
    objList.add(make_shared<Sphere>(100.0,Vector(0,-100.5,-1),material_ground));
    objList = random_scene();

    start = clock();
    op_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel(0,0,0); 
            for (int sample = 0; sample < spp; sample++){
                auto u = (double(i) + random_double())  / (image_width-1);
                auto v = (double(j) + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel += ray_color(r,objList,maxDepth);
            }
            write_color(op_file, pixel,spp);
        }
    }
    op_file.close();
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "Time taken to RayTrace is  : "<<time_taken;
    std::cerr << "\nDone.\n";
    return 0;
}