#include "material.h"
#include "hitinfos.h"
#include "ray.h"
#include "maths.h"

bool MaterialLambert::Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const
{
    auto target = hit_infos.Point + hit_infos.Normal + RandomInUnitSphere( state );
    scattered_ray = Ray( hit_infos.Point, Normalize( target - hit_infos.Point ) );
    attenuation = Albedo;

    return true;
}

bool MaterialMetal::Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const
{
    auto reflected = Reflect( ray.direction, hit_infos.Normal );
    scattered_ray = Ray( hit_infos.Point, Normalize( reflected + Fuzzyness * RandomInUnitSphere( state ) ) );
    attenuation = Albedo;

    return Vec3::Dot( scattered_ray.direction, hit_infos.Normal ) > 0.0f;
}

bool MaterialDiElectric::Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const
{
    Vec3 outward_normal;
    Vec3 reflected = Reflect( ray.direction, hit_infos.Normal );
    float ni_over_nt;
    attenuation = Vec3( 1.0f, 1.0f, 1.0f );
    Vec3 refracted;
    float reflect_prob;
    float cosine;

    if ( Vec3::Dot( ray.direction, hit_infos.Normal ) > 0.0f )
    {
        outward_normal = -hit_infos.Normal;
        ni_over_nt = RefractionIndex;
        cosine = RefractionIndex * Vec3::Dot( ray.direction, hit_infos.Normal );
    }
    else
    {
        outward_normal = hit_infos.Normal;
        ni_over_nt = 1.0f / RefractionIndex;
        cosine = -Vec3::Dot( ray.direction, hit_infos.Normal );
    }

    if ( Refract( ray.direction, outward_normal, ni_over_nt, refracted ) )
    {
        reflect_prob = Schlick( cosine, RefractionIndex );
    }
    else
    {
        reflect_prob = 1.0f;
    }

    if ( RandomFloat01( state ) < reflect_prob )
    {
        scattered_ray = Ray( hit_infos.Point, Normalize( reflected ) );
    }
    else
    {
        scattered_ray = Ray( hit_infos.Point, Normalize( refracted ) );
    }

    return true;
}