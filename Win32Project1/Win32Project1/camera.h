#pragma once

#include <stdint.h>

#include "json.hpp"
using nlohmann::json;

#include "ray.h"

struct CameraParameters
{
    Vec3 lookFrom,
        lookAt,
        up;
    float fov,
        aspectRatio,
        aperture,
        focusDistance;
};

class Camera
{
public:

    Camera( const CameraParameters & parameters );

    Ray GetRay( float u, float v, uint32_t& state ) const;

private:

    Vec3 origin, lowerLeftCorner, horizontal, vertical;
    Vec3 u, v, w;
    float lensRadius;
};

void from_json( const json & j, CameraParameters & p );