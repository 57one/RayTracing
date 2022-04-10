#ifndef AABB_H
#define AABB_H

#include "rtweekend.hpp"

class aabb {
    public:
        aabb() {}
        aabb(const Point3d& a, const Point3d& b) { minimum = a; maximum = b;}

        Point3d min() const {return minimum; }
        Point3d max() const {return maximum; }

        bool hit(const ray& r, double t_min, double t_max) const {
            for (int a = 0; a < 3; a++) {
                auto t0 = fmin((minimum[a] - r.origin()[a]) / r.direction()[a],
                               (maximum[a] - r.origin()[a]) / r.direction()[a]);
                auto t1 = fmax((minimum[a] - r.origin()[a]) / r.direction()[a],
                               (maximum[a] - r.origin()[a]) / r.direction()[a]);
                t_min = fmax(t0, t_min);
                t_max = fmin(t1, t_max);
                if (t_max <= t_min)
                    return false;
            }
            return true;
        }

        Point3d minimum;
        Point3d maximum;
};

#endif