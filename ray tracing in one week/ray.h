#ifndef __RAY_H
#define __RAY_H

#include"vec.h"
class ray{
public:
    ray() { }
    ray(const Point3d &origin, const Vec3d &direction):orig(origin), dir(direction) { }

    Point3d at(double t) const { return orig + dir * t; }
public:
    Point3d orig;
    Vec3d dir;
};

#endif
