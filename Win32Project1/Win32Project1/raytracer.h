#pragma once

#include "vec3.h"

class Backbuffer;
struct Ray;
class RayTracer
{
public:

    void Process( Backbuffer & back_buffer );

private:

    Vec3 Color( const Ray & ray ) const;
    float HitSphere( const Vec3 & center, float radius, const Ray & ray ) const;
};