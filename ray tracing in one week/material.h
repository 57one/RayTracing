#ifndef __MATERIAL_H
#define __MATERIAL_H

#include "rtweekend.h"

struct hit_record;

class material {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, Color3d& attenuation, ray& scattered
        ) const = 0;
};

#endif