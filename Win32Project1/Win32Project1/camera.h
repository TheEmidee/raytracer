#pragma once

#include <stdint.h>
#include "ray.h"

class Camera
{
public:

    Camera( const Vec3 & look_from, const Vec3 & look_at, const Vec3 & up, float vfov, float aspect, float aperture, float focus_distance );

    Ray GetRay( float u, float v, uint32_t& state ) const;

private:

    Vec3 origin, lowerLeftCorner, horizontal, vertical;
    Vec3 u, v, w;
    float lensRadius;
};