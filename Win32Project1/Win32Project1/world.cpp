#include "world.h"

#include "ray.h"
#include "hitinfos.h"
#include "hitable.h"

World::World( const std::vector< std::shared_ptr< Hitable > > & hitables_ )
    : hitables( hitables_ )
{

}

bool World::Hit( const Ray & ray, float min_time, float max_time, HitInfos & hit_infos ) const
{
    HitInfos temp_hit_infos;
    bool hit_anything = false;
    double closest_so_far = max_time;

    for ( const auto & hitable : hitables )
    {
        if ( hitable->Hit( ray, min_time, closest_so_far, temp_hit_infos ) )
        {
            hit_anything = true;
            closest_so_far = temp_hit_infos.Time;
            hit_infos = temp_hit_infos;
        }
    }

    return hit_anything;
}