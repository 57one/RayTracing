#ifndef __METAL_H
#define __METAL_H

#include"material.h"
#include"ray.h"
#include"vec.h"
#include"hittable.h"

class metal : public material {
    public:
        metal(const Color3d& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, Color3d& attenuation, ray& scattered
        ) const override {
            Vec3d reflected = reflect(unit_vector(r_in.dir), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere<double>());
            attenuation = albedo;
            return (dot(scattered.dir, rec.normal) > 0);
        }

    public:
        Color3d albedo;
        double fuzz;
};

#endif