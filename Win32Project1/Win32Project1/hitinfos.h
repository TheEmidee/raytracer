#pragma once

#include <memory>
#include "vec3.h"

class Material;

struct HitInfos
{
    float Time;
    Vec3 Point;
    Vec3 Normal;
    const Material * Material;
};