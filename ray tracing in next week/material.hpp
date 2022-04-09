#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.hpp"
#include "rtweekend.hpp"

struct hit_record;

class material {
   public:
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, Color3d& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
   public:
    lambertian(const Color3d& a) : albedo(a) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, Color3d& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector<double>();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction, r_in.time());
        attenuation = albedo;
        return true;
    }

   public:
    Color3d albedo;
};

class metal : public material {
   public:
    metal(const Color3d& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, Color3d& attenuation, ray& scattered) const override {
        Vec3d reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere<double>(), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

   public:
    Color3d albedo;
    double fuzz;
};

class dielectric : public material {
   public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, Color3d& attenuation, ray& scattered) const override {
        attenuation = Color3d(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        Vec3d unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3d direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, direction, r_in.time());
        return true;
    }

   public:
    double ir;  // Index of Refraction

   private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif