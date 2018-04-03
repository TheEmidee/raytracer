#include "material.h"
#include "hitinfos.h"
#include "ray.h"
#include "maths.h"

bool MaterialLambert::Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const
{
    auto target = hit_infos.Point + hit_infos.Normal + RandomInUnitSphere( state );
    scattered_ray.origin = hit_infos.Point;
    scattered_ray.direction = Normalize( target - hit_infos.Point );
    attenuation = Albedo;

    return true;
}

bool MaterialMetal::Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const
{
    auto reflected = Reflect( ray.direction, hit_infos.Normal );
    scattered_ray.origin = hit_infos.Point;
    scattered_ray.direction = reflected;
    attenuation = Albedo;

    return Vec3::Dot( scattered_ray.direction, hit_infos.Normal ) > 0.0f;
}