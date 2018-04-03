#pragma once

#include <stdint.h>
#include "vec3.h"

struct Ray;
struct HitInfos;

class Material
{
public:

    virtual bool Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const = 0;
};

class MaterialLambert : public Material
{
public:

    MaterialLambert( const Vec3 & albedo )
        : Albedo( albedo )
    { }

    virtual bool Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const override;

    Vec3 Albedo;
};

class MaterialMetal : public Material
{
public:

    MaterialMetal( const Vec3 & albedo )
        : Albedo( albedo )
    {
    }

    virtual bool Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const override;

    Vec3 Albedo;
};