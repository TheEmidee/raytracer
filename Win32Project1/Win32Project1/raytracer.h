#pragma once

#include <stdint.h>

#include "vec3.h"

class Backbuffer;
class World;
class Camera;
struct Ray;

class RayTracer
{

public:

    RayTracer( int sample_per_pixel, int max_trace_depth )
        : samplePerPixel( sample_per_pixel ), maxTraceDepth( max_trace_depth )
    {}

    void Process( Backbuffer & back_buffer, int & ray_count, const World & world, const Camera & camera );

private:

    Vec3 Trace( int & ray_count, const Ray & ray, const World & world, uint32_t & state, int depth ) const;

    int samplePerPixel;
    int maxTraceDepth;
};