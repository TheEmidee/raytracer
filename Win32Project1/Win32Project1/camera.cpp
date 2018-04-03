#include "camera.h"
#include "vec3.h"
#include "maths.h"

Camera::Camera( const Vec3 & look_from, const Vec3 & look_at, const Vec3 & up, float vfov, float aspect, float aperture, float focus_distance )
{
    lensRadius = aperture / 2.0f;

    float theta = vfov * kPI / 180.0f;
    float half_height = tanf( theta / 2.0f );
    float half_width = aspect * half_height;

    origin = look_from;
    w = Normalize( look_from - look_at );
    u = Normalize( Vec3::Cross( up, w ) );
    v = Vec3::Cross( w, u );

    lowerLeftCorner = origin - half_width * focus_distance * u - half_height * focus_distance * v - focus_distance * w;
    horizontal = 2.0f * half_width * focus_distance * u;
    vertical = 2.0f * half_height * focus_distance * v;
}

Ray Camera::GetRay( float s, float t, uint32_t & state ) const
{
    Vec3 rd = lensRadius * RandomInUnitDisk( state );
    Vec3 offset = u * rd.x + v * rd.y;

    return Ray( origin + offset, Normalize( lowerLeftCorner + s * horizontal + t * vertical - origin - offset ) );
}