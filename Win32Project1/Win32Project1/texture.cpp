#include "texture.h"

Vec3 TextureConstantColor::GetColor( float u, float v, const Vec3 & position ) const
{
    return color;
}