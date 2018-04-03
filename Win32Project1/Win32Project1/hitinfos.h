#pragma once

#include <memory>
#include "vec3.h"

class Material;

struct HitInfos
{
    float Time;
    Vec3 Point;
    Vec3 Normal;
    std::weak_ptr< Material > Material;
};