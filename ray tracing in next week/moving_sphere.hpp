#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "hittable.hpp"
#include "rtweekend.hpp"

class moving_sphere : public hittable {
   public:
    moving_sphere() {}
    moving_sphere(
        Point3d cen0, Point3d cen1, double _time0, double _time1, double r, shared_ptr<material> m)
        : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m){};

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    Point3d center(double time) const;

   public:
    Point3d center0, center1;
    double time0, time1;
    double radius;
    shared_ptr<material> mat_ptr;
};

Point3d moving_sphere::center(double time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    Vec3d oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
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
    auto outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif