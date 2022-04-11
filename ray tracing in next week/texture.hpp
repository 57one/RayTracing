#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.hpp"
#include "rtw_stb_image.hpp"
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

class image_texture : public texture {
   public:
    const static int bytes_per_pixel = 3;

    image_texture()
        : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

    image_texture(const char* filename) {
        auto components_per_pixel = bytes_per_pixel;

        data = stbi_load(
            filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data) {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~image_texture() {
        delete data;
    }

    virtual Color3d value(double u, double v, const Vec3d& p) const override {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (data == nullptr)
            return Color3d(0, 1, 1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        // Clamp integer mapping, since actual coordinates should be less than 1.0
        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return Color3d(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

   private:
    unsigned char* data;
    int width, height;
    int bytes_per_scanline;
};

#endif