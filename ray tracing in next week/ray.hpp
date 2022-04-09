#ifndef RAY_H
#define RAY_H

#include "vec.hpp"

class ray {
   public:
    ray() {}
    ray(const Point3d &origin, const Vec3d &direction, double time = 0.f)
        : orig(origin), dir(direction), tm(time) {
    }

    Point3d origin() const { return orig; }
    Vec3d direction() const { return dir; }
    double time() const { return tm; }

    Point3d at(double t) const {
        return orig + t * dir;
    }

   public:
    Point3d orig;
    Vec3d dir;
    double tm;
};

#endif