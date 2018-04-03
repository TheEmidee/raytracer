#define NOMINMAX

#include "raytracer.h"

#include <limits>

#include "backbuffer.h"
#include "vec3.h"
#include "ray.h"
#include "world.h"
#include "camera.h"
#include "hitinfos.h"
#include "maths.h"
#include "material.h"

void RayTracer::Process( Backbuffer & back_buffer, const World & world, const Camera & camera )
{
    uint32_t state = 1337;
    
    for ( int y = 0; y < back_buffer.GetHeight(); y++ )
    {
        float * data = back_buffer.GetData() + y * back_buffer.GetWidth() * 4;
        
        for ( int x = 0; x < back_buffer.GetWidth(); x++ )
        {
            Vec3 color( 0.0f, 0.0f, 0.0f );

            for ( int sample = 0; sample < camera.GetRaysPerPixel(); sample++ )
            {
                const auto u = static_cast< float >( x + RandomFloat01( state ) ) / static_cast< float >( back_buffer.GetWidth() );
                const auto v = static_cast< float >( y + RandomFloat01( state ) ) / static_cast< float >( back_buffer.GetHeight() );

                Ray ray = camera.GetRay( u, v );

                color += Color( ray, world, state, 0 );
            }
            
            color /= static_cast< float >( camera.GetRaysPerPixel() );
            color = Vec3( sqrt( color.x ), sqrt( color.y ), sqrt( color.z ) );

            data[ 0 ] = color.x;
            data[ 1 ] = color.y;
            data[ 2 ] = color.z;

            data += 4;
        }
    }
}

Vec3 RayTracer::Color( const Ray & ray, const World & world, uint32_t & state, int depth ) const
{
    HitInfos hit_infos;

    if ( world.Hit( ray, 0.0001f, std::numeric_limits< float >::max(), hit_infos ) )
    {
        Ray scattered;
        Vec3 attenuation;

        if ( depth < 50 )
        {
            if ( auto material_shared_ptr = hit_infos.Material.lock() )
            {
                if ( material_shared_ptr->Scatter( ray, hit_infos, attenuation, scattered, state ) )
                {
                    return attenuation * Color( scattered, world, state, depth + 1 );
                }
            }
        }
        
        return Vec3( 0.0f, 0.0f, 0.0f );
    }

    auto direction = Normalize( ray.direction );
    float t = 0.5f * ( direction.y + 1.0f );

    return ( 1.0f - t ) * Vec3::One() + t * Vec3( 0.5f, 0.7f, 1.0f );
}