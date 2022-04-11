#ifndef TEXTURE_H
#define TEXTURE_H

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
          : solid_color(Color3d(red,green,blue)) {}

        virtual Color3d value(double u, double v, const Vec3d& p) const override {
            return color_value;
        }

    private:
        Color3d color_value;
};

#endif