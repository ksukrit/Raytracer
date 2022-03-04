#ifndef MATERIAL_H
#define MATERIAL_H

struct hit_record;

Vector reflect(const Vector& v, const Vector& n){
    return v - n*2*dot(v,n);
}

class Material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, Vector& attenuation, ray& scattered) const = 0;
};


class Diffusion : public Material {
    public:
        Diffusion(const Vector& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, Vector& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + randomPoint();

            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;


            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        Vector albedo;
};

class Metal :public Material {
    Vector albedo;
    double fuzz;
public:
    Metal(const Vector &a) : albedo(a) {}
    Metal(const Vector &a,double f) : albedo(a),fuzz(f<1?f:1) {}
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, Vector& attenuation, ray& scattered
        ) const override {
            Vector reflected = reflect(r_in.getDirection().normalize(), rec.normal);
            scattered = ray(rec.p, reflected + randomPoint()*fuzz);
            attenuation = albedo;
            return (dot(scattered.getDirection(), rec.normal) > 0);
        }

};

#endif