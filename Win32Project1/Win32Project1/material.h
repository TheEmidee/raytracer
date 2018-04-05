#pragma once

#include <stdint.h>
#include <algorithm>

#include "vec3.h"

struct Ray;
struct HitInfos;

class Material
{
public:

    Material() {}
    ~Material() {}

    virtual bool Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const = 0;
};

class MaterialLambert : public Material
{
public:

    MaterialLambert() {}

    MaterialLambert( const Vec3 & albedo )
        : Albedo( albedo )
    {
    }

    virtual bool Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const override;

private:

    Vec3 Albedo;
};

class MaterialMetal : public Material
{
public:

    MaterialMetal() {}

    MaterialMetal( const Vec3 & albedo, float fuzzyness )
        : Albedo( albedo )
    {
        Fuzzyness = fuzzyness > 1.0f ? 1.0f : fuzzyness;
    }

    virtual bool Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const override;

private:

    Vec3 Albedo;
    float Fuzzyness;
};

class MaterialDiElectric : public Material
{
public:

    MaterialDiElectric() {}

    MaterialDiElectric( float refraction_index )
        : RefractionIndex( refraction_index )
    {
    }

    virtual bool Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const override;

private:

    float RefractionIndex;
};