#ifndef __LAMBERTIAN_H
#define __LAMBERTIAN_H

#include"material.h"
#include"vec.h"
#include"hittable.h"

class lambertian : public material {
    public:
        lambertian(const Color3d& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, Color3d& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + random_unit_vector<double>();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        Color3d albedo;
};

#endif