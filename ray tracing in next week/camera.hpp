#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.hpp"

class camera {
   public:
    camera(
        Point3d lookfrom,
        Point3d lookat,
        Vec3d vup,
        double vfov,  // vertical field-of-view in degrees
        double aspect_ratio,
        double aperture,
        double focus_dist,
        double _time0 = 0,
        double _time1 = 0) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vector(lookfrom - lookat);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

        lens_radius = aperture / 2;
        time0 = _time0;
        time1 = _time1;
    }

    ray get_ray(double s, double t) const {
        Vec3d rd = lens_radius * random_in_unit_disk<double>();
        Vec3d offset = u * rd.x + v * rd.y;

        return ray(
            origin + offset,
            lower_left_corner + s * horizontal + t * vertical - origin - offset,
            random_double(time0, time1));
    }

   private:
    Point3d origin;
    Point3d lower_left_corner;
    Vec3d horizontal;
    Vec3d vertical;
    Vec3d u, v, w;
    double lens_radius;
    double time0, time1;  // shutter open/close times
};
#endif