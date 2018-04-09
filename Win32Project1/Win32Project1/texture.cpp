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

Vec3 TexturePerlinNoise::GetColor( float u, float v, const Vec3 & position ) const
{
    return Vec3( 1.0f, 1.0f, 1.0f ) * 0.5f * ( 1 + sin( noiseScale * position.z + 10.0f * noise->GetTurbulence( position ) ) );
}