#ifndef RAY_H
#define RAY_H

// Ideally should import vector.h
class ray{
    Vector origin;
    Vector direction;
public:
    ray(){}
    ray(Vector origin, Vector direction):origin(origin), direction(direction){}
    Vector getOrigin() const {
        return origin;
    }
    Vector getDirection() const {
        return direction;
    }

    Vector at(double t) const {
        return origin + direction*t;
    }

};

#endif