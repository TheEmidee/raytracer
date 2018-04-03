#include "camera.h"
#include "vec3.h"
#include "maths.h"

Camera::Camera( const Vec3 & look_from, const Vec3 & look_at, const Vec3 & up, float vfov, float aspect, int rays_per_pixel )
    : raysPerPixel( rays_per_pixel )
{
    Vec3 u, v, w;

    float theta = vfov * kPI / 180.0f;
    float half_height = tanf( theta / 2.0f );
    float half_width = aspect * half_height;

    origin = look_from;
    w = Normalize( look_from - look_at );
    u = Normalize( Vec3::Cross( up, w ) );
    v = Vec3::Cross( w, u );

    lower_left_corner = origin - half_width * u - half_height * v - w;
    horizontal = 2.0f * half_width * u;
    vertical = 2.0f * half_height * v;
}

Ray Camera::GetRay( float u, float v ) const
{
    return Ray( origin, Normalize( lower_left_corner + u * horizontal + v * vertical ) );
}