#pragma once

#include <stdint.h>
#include <memory>

#include "enkiTS/TaskScheduler.h"

#include "json.hpp"
using nlohmann::json;

#include "vec3.h"
#include "world.h"
#include "camera.h"

struct Ray;

struct RayTracerParameters
{
    int width;
    int height;
    int samplePerPixel;
    int maxTraceDepth;
    int frameCount;
};

class RayTracer
{
public:

    RayTracer(const RayTracerParameters & parameters, std::unique_ptr<Camera> camera_, std::unique_ptr<World> world_);

    void Process(int & ray_count);

    int width;
    int height;
    float * data;
    int frameIndex;

private:

    int frameCount;
    int samplePerPixel;
    int maxTraceDepth;
    enki::TaskScheduler taskScheduler;
    std::unique_ptr<const Camera> camera;
    std::unique_ptr<const World> world;
};

void from_json( const json & j, RayTracerParameters & p );