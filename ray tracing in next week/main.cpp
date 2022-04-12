#include <iostream>

#include "aarect.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "moving_sphere.hpp"
#include "rtweekend.hpp"
#include "sphere.hpp"

hittable_list random_scene() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(Color3d(0.2, 0.3, 0.1), Color3d(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(Point3d(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    // auto ground_material = make_shared<lambertian>(Color3d(0.5, 0.5, 0.5));
    // world.add(make_shared<sphere>(Point3d(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3d center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3d(4, 0.2, 0)).norm() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color3d::random() * Color3d::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + Vec3d(0, random_double(0, .5), 0);
                    world.add(make_shared<moving_sphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color3d::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(Point3d(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(Color3d(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(Point3d(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(Color3d(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(Point3d(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(Color3d(0.2, 0.3, 0.1), Color3d(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(Point3d(0, -10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(Point3d(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}

hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(Point3d(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(Point3d(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return objects;
}

hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(Point3d(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}

hittable_list simple_light() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(Point3d(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(Point3d(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(Color3d(4, 4, 4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

double hit_sphere(const Point3d& center, double radius, const ray& r) {
    Vec3d oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

Color3d ray_color(const ray& r, const Color3d& background, const hittable& world, int depth) {
    hit_record rec;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Color3d(0, 0, 0);
    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    Color3d attenuation;
    Color3d emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

int main() {
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    int samples_per_pixel = 100;
    const int max_depth = 50;

    // World

    hittable_list world;

    Point3d lookfrom;
    Point3d lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    Color3d background(0, 0, 0);

    switch (0) {
        case 1:
            world = random_scene();
            background = Color3d(0.70, 0.80, 1.00);
            lookfrom = Point3d(13, 2, 3);
            lookat = Point3d(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;
            break;

        case 2:
            world = two_spheres();
            background = Color3d(0.70, 0.80, 1.00);
            lookfrom = Point3d(13, 2, 3);
            lookat = Point3d(0, 0, 0);
            vfov = 20.0;
            break;
        case 3:
            world = two_perlin_spheres();
            background = Color3d(0.70, 0.80, 1.00);
            lookfrom = Point3d(13, 2, 3);
            lookat = Point3d(0, 0, 0);
            vfov = 20.0;
            break;
        case 4:
            world = earth();
            background = Color3d(0.70, 0.80, 1.00);
            lookfrom = Point3d(13, 2, 3);
            lookat = Point3d(0, 0, 0);
            vfov = 20.0;
            break;
        default:
        case 5:
            world = simple_light();
            samples_per_pixel = 400;
            background = Color3d(0, 0, 0);
            lookfrom = Point3d(26, 3, 6);
            lookat = Point3d(0, 2, 0);
            vfov = 20.0;
            break;
    }

    // Camera

    Vec3d vup(0, 1, 0);
    auto dist_to_focus = 10.0;

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    // Render

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color3d pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}