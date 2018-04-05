#pragma once

#include <memory>

#include "json.hpp"
using nlohmann::json;

#include "vec3.h"
#include "material.h"

struct Ray;
struct HitInfos;
class aabb;

class Hitable
{
public:

    Hitable() {}

    Hitable( std::shared_ptr< const Material > material_ );
    // Hitable( const json & material_json );

    virtual ~Hitable() = default;
    virtual bool Hit( const Ray & ray, float min_time, float max_time, HitInfos & hit_infos ) const = 0;
    virtual bool GetBoundingBox(aabb & box) const = 0;

protected:

    std::shared_ptr< const Material > material;
};