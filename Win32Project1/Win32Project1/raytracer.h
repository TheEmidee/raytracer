#pragma once

#include <stdint.h>

#include "enkiTS/TaskScheduler.h"
#include "vec3.h"

class Backbuffer;
class World;
class Camera;
struct Ray;

class RayTracer
{
public:

    RayTracer(int sample_per_pixel, int max_trace_depth);

    void Process( Backbuffer & back_buffer, int & ray_count, const int frame_count, const World & world, const Camera & camera );

private:

    int samplePerPixel;
    int maxTraceDepth;
    enki::TaskScheduler taskScheduler;
};