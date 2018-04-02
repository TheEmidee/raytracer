#include "raytracer.h"
#include "backbuffer.h"
#include "vec3.h"
#include "ray.h"

void RayTracer::Process( Backbuffer & back_buffer )
{
    Vec3 lower_left( -2.0f, -1.0f, -1.0f );
    Vec3 horizontal( 4.0, 0.0f, 0.0f );
    Vec3 vertical( 0.0f, 2.0f, 0.0f );
    Vec3 origin;
    
    for ( int y = 0; y < back_buffer.GetHeight(); y++ )
    {
        float * data = back_buffer.GetData() + y * back_buffer.GetWidth() * 4;
        
        for ( int x = 0; x < back_buffer.GetWidth(); x++ )
        {
            const auto u = static_cast< float >( x ) / static_cast< float >( back_buffer.GetWidth() );
            const auto v = static_cast< float >( y ) / static_cast< float >( back_buffer.GetHeight() );

            Ray ray( origin, Normalize( lower_left + u * horizontal + v * vertical ) );
            Vec3 color = Color( ray );

            data[ 0 ] = color.x;
            data[ 1 ] = color.y;
            data[ 2 ] = color.z;

            data += 4;
        }
    }
}

Vec3 RayTracer::Color( const Ray & ray ) const
{
    auto t = HitSphere( Vec3( 0.0f, 0.0f, -1.0f ), 0.5f, ray );

    if ( t > 0.0f )
    {
        auto normal = Normalize( ray.PointAt( t ) - Vec3( 0.0f, 0.0f, -1.0f ) );
        return 0.5f * ( normal + 1.0f );
    }

    auto direction = Normalize( ray.direction );
    t = 0.5f * ( direction.y + 1.0f );

    return ( 1.0f - t ) * Vec3::One() + t * Vec3( 0.5f, 0.7f, 1.0f );
}

float RayTracer::HitSphere( const Vec3 & center, float radius, const Ray & ray ) const
{
    Vec3 origin_to_center = ray.origin - center;
    float a = Vec3::Dot( ray.direction, ray.direction );
    float b = 2.0f * Vec3::Dot( origin_to_center, ray.direction );
    float c = Vec3::Dot( origin_to_center, origin_to_center ) - radius * radius;
    float discriminant = b*b - 4 * a*c;

    if ( discriminant < 0.0f )
    {
        return -1.0f;
    }

    return ( -b - sqrt( discriminant ) ) / ( 2.0f * a );
}