#include "texture.h"

Vec3 TextureConstantColor::GetColor( float u, float v, const Vec3 & position ) const
{
    return color;
}

Vec3 TextureChecker::GetColor( float u, float v, const Vec3 & position ) const
{
    float sine = sinf( squareSize * position.x ) * sinf( squareSize * position.y ) * sinf( squareSize * position.z );

    return sine < 0.0f ? color1 : color2;
}