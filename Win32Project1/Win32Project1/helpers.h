#pragma once

#include "vec3.h"

inline void AssertUnit( const Vec3 & v )
{
    assert( fabsf( v.sqLength() - 1.0f ) < 0.01f );
}