#pragma once

#include "vec3.h"

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