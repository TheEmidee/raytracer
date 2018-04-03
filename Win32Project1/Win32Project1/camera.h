#pragma once

#include "ray.h"

class Camera
{
public:

    Camera( int rays_per_pixel );

    Ray GetRay( float u, float v ) const;
    inline int GetRaysPerPixel() const { return raysPerPixel; }

private:

    Vec3 origin, lower_left_corner, horizontal, vertical;
    int raysPerPixel;
};