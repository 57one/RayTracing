#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.hpp"
#include "rtweekend.hpp"

class texture {
   public:
    virtual Color3d value(double u, double v, const Point3d& p) const = 0;
};

class solid_color : public texture {
   public:
    solid_color() {}
    solid_color(Color3d c) : color_value(c) {}

    solid_color(double red, double green, double blue)
        : solid_color(Color3d(red, green, blue)) {}

    virtual Color3d value(double u, double v, const Vec3d& p) const override {
        return color_value;
    }

   private:
    Color3d color_value;
};

class checker_texture : public texture {
   public:
    checker_texture() {}

    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
        : even(_even), odd(_odd) {}

    checker_texture(Color3d c1, Color3d c2)
        : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

    virtual Color3d value(double u, double v, const Point3d& p) const override {
        auto sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

   public:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;
};

class noise_texture : public texture {
   public:
    noise_texture() {}
    noise_texture(double sc) : scale(sc) {}
    virtual Color3d value(double u, double v, const Point3d& p) const override {
        return Color3d(1, 1, 1) * 0.5 * (1 + sin(scale * p.z + 10 * noise.turb(p)));
    }

   public:
    perlin noise;
    double scale;
};

#endif