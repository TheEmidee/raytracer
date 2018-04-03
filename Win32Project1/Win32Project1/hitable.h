#pragma once

#include "vec3.h"

struct Ray;
struct HitInfos;

class Hitable
{
public:

    virtual ~Hitable() = default;
    virtual bool Hit( const Ray & ray, float min_time, float max_time, HitInfos & hit_infos ) const = 0;
};