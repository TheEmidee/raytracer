#include "camera.h"

#include "vec3.h"
#include "maths.h"

Camera::Camera( const CameraParameters & parameters )
{
    lensRadius = parameters.aperture / 2.0f;

    float theta = parameters.fov * kPI / 180.0f;
    float half_height = tanf( theta / 2.0f );
    float half_width = parameters.aspectRatio * half_height;

    origin = parameters.lookFrom;
    w = Normalize( origin - parameters.lookAt );
    u = Normalize( Vec3::Cross( parameters.up, w ) );
    v = Vec3::Cross( w, u );

    lowerLeftCorner = origin - half_width * parameters.focusDistance * u - half_height * parameters.focusDistance * v - parameters.focusDistance * w;
    horizontal = 2.0f * half_width * parameters.focusDistance * u;
    vertical = 2.0f * half_height * parameters.focusDistance * v;
}

Ray Camera::GetRay( float s, float t, uint32_t & state ) const
{
    Vec3 rd = lensRadius * RandomInUnitDisk( state );
    Vec3 offset = u * rd.x + v * rd.y;

    return Ray( origin + offset, Normalize( lowerLeftCorner + s * horizontal + t * vertical - origin - offset ) );
}

void from_json( const json& j, CameraParameters & p )
{
    p.lookFrom = j.at( "lookFrom" ).get<Vec3>();
    p.lookAt = j.at( "lookAt" ).get<Vec3>();
    p.up = j.at( "up" ).get<Vec3>();
    p.fov = j.at( "fov" ).get<float>();
    p.aspectRatio = j.at( "aspectRatio" ).get<float>();
    p.aperture = j.at( "aperture" ).get<float>();
    p.focusDistance = j.at( "focusDistance" ).get<float>();
}