#pragma once

#include <vector>
#include <memory>

#include "hitable.h"

struct Ray;
struct HitInfos;

class World
{
public:

    World( const std::vector< std::shared_ptr< Hitable > > & hitables_ );

    bool Hit( const Ray & ray, float min_time, float max_time, HitInfos & hit_infos ) const;

private:

    std::vector< std::shared_ptr< Hitable > > hitables;
};