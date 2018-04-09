#define NOMINMAX

#include "raytracer.h"

#include <limits>
#include <atomic>

#include "vec3.h"
#include "ray.h"
#include "world.h"
#include "camera.h"
#include "hitinfos.h"
#include "maths.h"
#include "material.h"

Vec3 Trace( int & ray_count, const Ray & ray, const World & world, uint32_t & state, int depth, int max_trace_depth )
{
    HitInfos hit_infos;

    ray_count++;

    if ( world.Hit( ray, 0.0001f, std::numeric_limits< float >::max(), hit_infos ) )
    {
        Ray scattered;
        Vec3 attenuation;

        if ( depth < max_trace_depth )
        {
            if ( hit_infos.Material->Scatter( ray, hit_infos, attenuation, scattered, state ) )
            {
                return attenuation * Trace( ray_count, scattered, world, state, depth + 1, max_trace_depth );
            }
        }

        return Vec3( 0.0f, 0.0f, 0.0f );
    }

    auto direction = Normalize( ray.direction );
    float t = 0.5f * ( direction.y + 1.0f );

    return ( 1.0f - t ) * Vec3::One() + t * Vec3( 0.5f, 0.7f, 1.0f );
}

struct RayTracerTaskSetData
{
    float * backBufferData;
    int backBufferWidth;
    int backBufferHeight;
    int frameCount;
    int samplePerPixel;
    const Camera & camera;
    const World & world;
    int maxTraceDepth;
};

struct RayTracerTaskSet : enki::ITaskSet
{
    RayTracerTaskSet( uint32_t setSize_, uint32_t minRange_, const RayTracerTaskSetData & data_ )
        : enki::ITaskSet( setSize_, minRange_ ), taskSetdata( data_ ), rayCount( 0 )
    {
    }

    virtual void ExecuteRange( enki::TaskSetPartition range, uint32_t threadnum )
    {
        float lerpFac = float( taskSetdata.frameCount ) / float( taskSetdata.frameCount + 1 );

        int ray_count = 0;

        for ( auto y = range.start; y < range.end; y++ )
        {
            float * data = taskSetdata.backBufferData + y * taskSetdata.backBufferWidth * 4;
            uint32_t state = ( y * 9781 + taskSetdata.frameCount * 6271 ) | 1;

            for ( int x = 0; x < taskSetdata.backBufferWidth; x++ )
            {
                Vec3 color( 0.0f, 0.0f, 0.0f );

                for ( int sample = 0; sample < taskSetdata.samplePerPixel; sample++ )
                {
                    const auto u = static_cast< float >( x + RandomFloat01( state ) ) / static_cast< float >( taskSetdata.backBufferWidth );
                    const auto v = static_cast< float >( y + RandomFloat01( state ) ) / static_cast< float >( taskSetdata.backBufferHeight );

                    Ray ray = taskSetdata.camera.GetRay( u, v, state );

                    color += Trace( ray_count, ray, taskSetdata.world, state, 0, taskSetdata.maxTraceDepth );
                }

                color /= static_cast< float >( taskSetdata.samplePerPixel );

                Vec3 prev( data[ 0 ], data[ 1 ], data[ 2 ] );
                color = prev * lerpFac + color * ( 1 - lerpFac );

                data[ 0 ] = color.x;
                data[ 1 ] = color.y;
                data[ 2 ] = color.z;
                data += 4;
            }
        }

        rayCount += ray_count;
    }

    int rayCount;

private:

    RayTracerTaskSetData taskSetdata;
};

RayTracer::RayTracer( const RayTracerParameters & parameters, std::unique_ptr<Camera> camera_, std::unique_ptr<World> world_ )
    : width( parameters.width )
    , height( parameters.height )
    , samplePerPixel( parameters.samplePerPixel )
    , maxTraceDepth( parameters.maxTraceDepth )
    , frameCount( parameters.frameCount )
    , frameIndex( 0 )
    , camera( std::move( camera_ ) )
    , world( std::move( world_ ) )
{
    taskScheduler.Initialize();

    data = new float[ width * height * 4 ];
    memset( data, 0, width * height * 4 * sizeof( data[ 0 ] ) );
}

void RayTracer::Process( int & ray_count )
{
    RayTracerTaskSetData task_set_data
    {
        data,
        width,
        height,
        frameIndex,
        samplePerPixel,
        *( camera.get() ),
        *( world.get() ),
        maxTraceDepth
    };

    const auto set_size = static_cast< uint32_t >( height );
    const auto threaded = true;
    const auto min_range = threaded ? 4 : set_size;

    RayTracerTaskSet task( height, set_size, task_set_data );

    taskScheduler.AddTaskSetToPipe( &task );

    // wait for task set (running tasks if they exist) - since we've just added it and it has no range we'll likely run it.
    taskScheduler.WaitforTaskSet( &task );

    ray_count = task.rayCount;
    frameIndex++;
}

bool RayTracer::Finished() const
{
    return frameIndex == frameCount;
}

void from_json( const json& j, RayTracerParameters & p )
{
    p.width = j.at( "width" ).get<int>();
    p.height = j.at( "height" ).get<int>();
    p.samplePerPixel = j.at( "samplePerPixel" ).get<int>();
    p.maxTraceDepth = j.at( "maxTraceDepth" ).get<int>();
    p.frameCount = j.at( "frameCount" ).get<int>();
}