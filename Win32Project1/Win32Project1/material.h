#pragma once

#include <stdint.h>
#include <algorithm>

#include "vec3.h"

struct Ray;
struct HitInfos;
class Texture;

class Material
{
public:

    ~Material() {}

    virtual bool Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const = 0;
};

class MaterialLambert : public Material
{
public:

    MaterialLambert( std::shared_ptr< Texture > texture_ )
        : texture( texture_ )
    {}

    virtual bool Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const override;

private:

    std::shared_ptr< Texture > texture;
};

class MaterialMetal : public Material
{
public:

    MaterialMetal( std::shared_ptr< Texture > texture_, float roughness_ )
        : texture( texture_ )
    {
        roughness = roughness_ > 1.0f ? 1.0f : roughness_;
    }

    virtual bool Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const override;

private:

    std::shared_ptr< Texture > texture;
    float roughness;
};

class MaterialDiElectric : public Material
{
public:

    MaterialDiElectric( float refraction_index )
        : RefractionIndex( refraction_index )
    {
    }

    virtual bool Scatter( const Ray & ray, const HitInfos & hit_infos, Vec3 & attenuation, Ray & scattered_ray, uint32_t & state ) const override;

private:

    float RefractionIndex;
};