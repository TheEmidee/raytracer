#pragma once

#include "hitable.h"

#include <vector>
#include <memory>

#include "aabb.h"

struct Ray;
struct HitInfos;

class BVHNode : public Hitable
{
public:

    BVHNode(const std::vector< std::shared_ptr< Hitable > > & hitables_, uint32_t & state);

    virtual bool Hit(const Ray & ray, float min_time, float max_time, HitInfos & hit_infos) const override;
    virtual bool GetBoundingBox(aabb & box) const override;

    std::shared_ptr< Hitable > left;
    std::shared_ptr< Hitable > right;
    aabb box;
};