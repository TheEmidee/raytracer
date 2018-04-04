#include "world.h"

#include "ray.h"
#include "hitinfos.h"
#include "bvhnode.h"

World::World( const std::vector< std::shared_ptr< Hitable > > & hitables_, uint32_t & state)
{
    bvhNode = std::make_unique< BVHNode >(hitables_, state);
}

bool World::Hit( const Ray & ray, float min_time, float max_time, HitInfos & hit_infos ) const
{
    return bvhNode->Hit(ray, min_time, max_time, hit_infos);
}