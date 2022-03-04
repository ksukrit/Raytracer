#ifndef ObjectList_h
#define ObjectList_h
#include "Object.h"
#include "ray.h"
#include <vector>
#include <memory>
class ObjectList : public Object{
    std::vector<std::shared_ptr<Object>> objects;
public :
    ObjectList(){}
    ~ObjectList(){}

    void clear(){
        objects.clear();
    }

    int no_objects(){
        return objects.size();
    }

    void add(std::shared_ptr<Object> obj){
        objects.push_back(obj);
    }

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override{
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = t_max;
        for (auto& obj : objects){
            if (obj->hit(r, t_min, closest_so_far, temp_rec)){
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }

};
#endif