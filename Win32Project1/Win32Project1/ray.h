#pragma once

#include "helpers.h"

struct Ray
{
    Ray() {}
    Ray( const Vec3 & new_origin, const Vec3 & new_direction ) 
        : origin( new_origin ), direction( new_direction ) 
    { 
        AssertUnit( direction ); 
    }

    inline Vec3 PointAt( float t ) const { return origin + direction * t; }

    Vec3 origin, direction;
};