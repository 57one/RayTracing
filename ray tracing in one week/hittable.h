#ifndef __HITTABLE_H
#define __HITTABLE_H

#include"ray.h"
#include"vec.h"

class material;

struct hit_record {
    Point3d p;
    Vec3d normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const Vec3d& outward_normal) {
        front_face = dot(r.dir, outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif
