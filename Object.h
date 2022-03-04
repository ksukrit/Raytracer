#ifndef Object_H
#define Object_H
class Material;

struct hit_record {
    Vector p;
    Vector normal;
    double t;
    shared_ptr<Material> mat_ptr;
    bool front_face;
    void set_face_normal(const ray& r, Vector& outward_normal) {
        front_face = dot(r.getDirection(), outward_normal) < 0;
        normal = front_face ? outward_normal.normalize() :-outward_normal.normalize();
    }

};

class Object{
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;

};
#endif