#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
class Sphere : public Object{
    double radius;
    Vector center_pos,color_vec,emission_vec;
    ReflectionType rt;
    shared_ptr<Material> material;
public:
    Sphere(double radius, Vector center_pos, ReflectionType rt):radius(radius), center_pos(center_pos), rt(rt){
        color_vec = Vector(0.2, 0.7, 1.0);
        emission_vec = Vector(0, 0, 0);
    }
    Sphere(double radius, Vector center_pos, Vector color_vec, Vector emission_vec, ReflectionType rt):radius(radius), center_pos(center_pos), color_vec(color_vec), emission_vec(emission_vec), rt(rt){}
    
    Sphere(double radius,Vector center_pos,shared_ptr<Material>m ):radius(radius),center_pos(center_pos),material(m){}
    double getRadius() const {
        return radius;
    }

    Vector getCenterPos() const {
        return center_pos;
    }

    ReflectionType getReflectionType() const {
        return rt;
    }

    Vector getColor() const {
        return color_vec;
    }

    // Hit the sphere with a ray
    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override{
        Vector o_pos =  r.getOrigin() - center_pos;
        // get component of o_pos in direction of ray
        double a = dot(r.getDirection(), r.getDirection());
        double b = dot(o_pos,r.getDirection());
        double c = dot(o_pos,o_pos) - radius * radius;
        // solve the quadratic equation for finding hits
        auto discriminant = b*b - a*c;

        //double discriminant = b*b - o_pos.dot(o_pos) + radius*radius;
        if (discriminant < 0)
            return false;
        else{
            auto discriminant_root = std::sqrt(discriminant);
            auto root  = (-b - discriminant_root)/a;
            if(root > t_max || root < t_min){
                root = (-b + discriminant_root)/a;
                if(root > t_max || root < t_min)
                    return false;
            }
            
            rec.t = root;
            rec.p = r.at(rec.t);
            rec.mat_ptr = material;
            Vector outward_normal = (rec.p - center_pos) / radius;
            rec.set_face_normal(r, outward_normal);
            return true;
        }
    }
};
#endif