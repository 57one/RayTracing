#ifndef __SPHERE_H
#define __SPHERE_H

#include"hittable.h"
#include"vec.h"
#include"material.h"

class sphere: public hittable {
public:
    sphere() { }
    sphere(Point3d cent, double r, shared_ptr<material> m):center(cent), radius(r), mat_ptr(m) { }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
public:
    Point3d center;
    double radius;
    shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    Vec3d oc = r.orig - center;
    auto a = r.dir.length_squared();
    auto half_b = dot(oc, r.dir);
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Vec3d outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif