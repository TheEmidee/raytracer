#include "maths.h"
#include <random>

static uint32_t XorShift32( uint32_t& state )
{
    uint32_t x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 15;
    state = x;
    return x;
}

float RandomFloat01()
{
    static std::random_device rd;
    static std::mt19937 e2( rd() );
    static std::uniform_real_distribution < float  > dist( 0, 1 );

    return dist( e2 );
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

float Schlick( float cosine, float refraction_index )
{
    float r0 = ( 1.0f - refraction_index ) / ( 1.0f + refraction_index );
    r0 = r0 * r0;

    return r0 + ( 1.0f - r0 ) * powf( 1 - cosine, 0.5f );
}

float TrilinearInterpolate( Vec3 c[ 2 ][ 2 ][ 2 ], float u, float v, float w )
{
    float uu = u * u*( 3 - 2 * u );
    float vv = v * v*( 3 - 2 * v );
    float ww = w * w*( 3 - 2 * w );
    float accum = 0;
    
    for ( int i = 0; i < 2; i++ )
    {
        for ( int j = 0; j < 2; j++ )
        {
            for ( int k = 0; k < 2; k++ )
            {
                Vec3 weight_v( u - i, v - j, w - k );

                accum += ( i*uu + ( 1 - i )*( 1 - uu ) )
                    * ( j*vv + ( 1 - j )*( 1 - vv ) )
                    * ( k*ww + ( 1 - k )*( 1 - ww ) )
                    * Vec3::Dot( c[ i ][ j ][ k ], weight_v );
            }
        }
    }

    return accum;
}