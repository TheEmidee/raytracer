#pragma once

#include <vector>
#include <memory>

#include "bvhnode.h"

struct Ray;
struct HitInfos;

class BVHNode;

class World
{
public:

    World( const std::vector< std::shared_ptr< Hitable > > & hitables_, uint32_t & state);

    bool Hit( const Ray & ray, float min_time, float max_time, HitInfos & hit_infos ) const;

private:

    std::unique_ptr< BVHNode > bvhNode;
};