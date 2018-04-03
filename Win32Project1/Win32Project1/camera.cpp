#include "camera.h"
#include "vec3.h"

Camera::Camera( int rays_per_pixel )
    : raysPerPixel( rays_per_pixel )
    , origin( Vec3( 0.0f, 0.0f, 0.0f ) )
    , lower_left_corner( Vec3( -2.0f, -1.0f, -1.0f ) )
    , horizontal( Vec3( 4.0f, 0.0f, 0.0f ) )
    , vertical( Vec3( 0.0, 2.0f, 0.0f ) )
{
}

Ray Camera::GetRay( float u, float v ) const
{
    return Ray( origin, Normalize( lower_left_corner + u * horizontal + v * vertical ) );
}