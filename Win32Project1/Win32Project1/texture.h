#pragma once

#include "vec3.h"
#include "perlinnoise.h"

class Texture
{
public:

    virtual Vec3 GetColor( float u, float v, const Vec3 & position ) const = 0;
};

class TextureConstantColor : public Texture
{
public:

    TextureConstantColor( const Vec3 & color_ )
        : color( color_ )
    { }

    virtual Vec3 GetColor( float u, float v, const Vec3 & position ) const override;

    Vec3 color;
};

class TextureChecker : public Texture
{
public:

    TextureChecker( const Vec3 & color1_, const Vec3 & color2_ , float squareSize_ )
        : color1( color1_ ), color2( color2_ ), squareSize( squareSize_ )
    {
    }

    virtual Vec3 GetColor( float u, float v, const Vec3 & position ) const override;

    Vec3 color1;
    Vec3 color2;
    float squareSize;
};

class TexturePerlinNoise : public Texture
{
public:

    TexturePerlinNoise( float noise_scale )
        : noise( std::make_unique< PerlinNoise >( noise_scale ) )
    {
    }

    virtual Vec3 GetColor( float u, float v, const Vec3 & position ) const override;

private:

    std::unique_ptr< PerlinNoise > noise;
};