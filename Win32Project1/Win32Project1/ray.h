#pragma once

#include "helpers.h"

struct Ray
{
    Ray() {}
    Ray( const Vec3 & origin, const Vec3 & direction ) : origin( origin ), direction( direction ) { AssertUnit( dir ); }

    inline Vec3 PointAt( float t ) const { return origin + direction * t; }

    Vec3 origin, direction;
};