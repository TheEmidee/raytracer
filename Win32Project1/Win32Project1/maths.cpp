#include "maths.h"

static uint32_t XorShift32( uint32_t& state )
{
    uint32_t x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 15;
    state = x;
    return x;
}

float RandomFloat01( uint32_t& state )
{
    return ( XorShift32( state ) & 0xFFFFFF ) / 16777216.0f;
}

Vec3 RandomInUnitDisk( uint32_t& state )
{
    Vec3 p;
    do
    {
        p = 2.0 * Vec3( RandomFloat01( state ), RandomFloat01( state ), 0 ) - Vec3( 1, 1, 0 );
    } while ( Vec3::Dot( p, p ) >= 1.0 );
    return p;
}

Vec3 RandomInUnitSphere( uint32_t& state )
{
    Vec3 p;
    do
    {
        p = 2.0*Vec3( RandomFloat01( state ), RandomFloat01( state ), RandomFloat01( state ) ) - Vec3( 1, 1, 1 );
    } while ( p.SquareLength() >= 1.0 );
    return p;
}

Vec3 RandomUnitVector( uint32_t& state )
{
    float z = RandomFloat01( state ) * 2.0f - 1.0f;
    float a = RandomFloat01( state ) * 2.0f * kPI;
    float r = sqrtf( 1.0f - z * z );
    float x = r * cosf( a );
    float y = r * sinf( a );
    return Vec3( x, y, z );
}