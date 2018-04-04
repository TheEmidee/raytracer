#include "aabb.h"

#include <utility>

#include "ray.h"
#include "maths.h"

aabb::aabb( const Vec3 & min_, const Vec3 & max_ )
    : min( min_ ), max( max_ )
{
}

bool aabb::Hit(const Ray & ray, float min_time, float max_time) const
{
    if (!HitOnComponent(ray.direction.x, ray.origin.x, min.x, max.x, min_time, max_time ) )
    {
        return false;
    }

    if (!HitOnComponent(ray.direction.y, ray.origin.y, min.y, max.y, min_time, max_time))
    {
        return false;
    }

    if (!HitOnComponent(ray.direction.z, ray.origin.z, min.z, max.z, min_time, max_time))
    {
        return false;
    }

    return true;
}

bool aabb::HitOnComponent( float ray_direction_component, float ray_position_component, float min_component, float max_component, float min_time, float max_time ) const
{
    float inv_dir = 1.0f * ray_direction_component;
    float t0 = (min_component - ray_position_component) * inv_dir;
    float t1 = (max_component - ray_position_component) * inv_dir;

    if (inv_dir < 0.0f)
    {
        std::swap(t0, t1);
    }

    min_time = ffmax(t0, min_time);
    max_time = ffmin(t1, max_time);

    if (max_time < min_time)
    {
        return false;
    }

    return true;
}

aabb aabb::GetSurroundingAABB(const aabb & box0, const aabb & box1)
{
    Vec3 small
    (
        fmin(box0.min.x, box1.min.x),
        fmin(box0.min.y, box1.min.y),
        fmin(box0.min.z, box1.min.z)
    );
    Vec3 big
    (
        fmax(box0.max.x, box1.max.x),
        fmax(box0.max.y, box1.max.y),
        fmax(box0.max.z, box1.max.z)
    );

    return aabb(small, big);
}