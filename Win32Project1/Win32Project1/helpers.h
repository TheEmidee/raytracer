#pragma once

#include <assert.h>

#include "vec3.h"

inline void AssertUnit( const Vec3 & v )
{
    assert( fabsf( v.SquareLength() - 1.0f ) < 0.01f );
}