#pragma once

#include <math.h>
#include <assert.h>
#include <stdint.h>

#include "vec3.h"

#define kPI 3.1415926f

float RandomFloat01( uint32_t & state );
Vec3 RandomInUnitDisk( uint32_t & state );
Vec3 RandomInUnitSphere( uint32_t & state );
Vec3 RandomUnitVector( uint32_t & state );

float Schlick( float cosine, float refraction_index );