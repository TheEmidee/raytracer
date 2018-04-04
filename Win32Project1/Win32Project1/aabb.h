#pragma once

#include <algorithm>
#include "vec3.h"

struct Ray;

class aabb
{
public:

    aabb() {}
    aabb(const Vec3 & min_, const Vec3 & max_);

    bool Hit(const Ray & ray, float min_time, float max_time) const;

    static aabb GetSurroundingAABB(const aabb & box0, const aabb & box1);

    Vec3 min, max;

private:

    bool HitOnComponent(float ray_direction_component, float ray_position_component, float min_component, float max_component, float min_time, float max_time) const;
};