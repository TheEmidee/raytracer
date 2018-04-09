#include "perlinnoise.h"

#include "vec3.h"
#include "maths.h"

PerlinNoise::PerlinNoise( int resolution_ )
{
    resolution_ = std::max( 0, resolution_ );

    data.resize( resolution_ );
    permX.resize( resolution_ );
    permY.resize( resolution_ );
    permZ.resize( resolution_ );

    for ( int i = 0; i < data.size(); i++ )
    {
        data[ i ] = Normalize( Vec3( -1 + 2 * RandomFloat01(), -1 + 2 * RandomFloat01(), -1 + 2 * RandomFloat01() ) );
    }

    GeneratePermutations( permX );
    GeneratePermutations( permY );
    GeneratePermutations( permZ );
}

float PerlinNoise::GetNoise( const Vec3 & position ) const
{
    float i = floor( position.x );
    float j = floor( position.y );
    float k = floor( position.z );

    float u = position.x - i;
    float v = position.y - j;
    float w = position.z - k;

    Vec3 c[ 2 ][ 2 ][ 2 ];

    for ( int di = 0; di < 2; di++ )
    {
        for ( int dj = 0; dj < 2; dj++ )
        {
            for ( int dk = 0; dk < 2; dk++ )
            {
                c[ di ][ dj ][ dk ] = data[ permX[ ( static_cast< int >( i ) + di ) & 255 ] ^ permY[ ( static_cast< int >( j ) + dj ) & 255 ] ^ permZ[ ( static_cast< int >( k ) + dk ) & 255 ] ];
            }
        }
    }

    return TrilinearInterpolate( c, u, v, w );
}

float PerlinNoise::GetTurbulence( const Vec3 & position, int depth ) const
{
    float accum = 0;
    Vec3 temp_position = position;
    float weight = 1.0;

    for ( int i = 0; i < depth; i++ )
    {
        accum += weight * GetNoise( temp_position );
        weight *= 0.5;
        temp_position *= 2;
    }

    return fabs( accum );
}

void PerlinNoise::GeneratePermutations( std::vector< int > & permutations ) const
{
    for ( auto i = 0; i < permutations.size(); i++ )
    {
        permutations[ i ] = i;
    }

    Permute( permutations );
}

void PerlinNoise::Permute( std::vector< int > & permutations ) const
{
    for ( auto i = permutations.size() - 1; i > 0; i-- )
    {
        int target = static_cast< int >( RandomFloat01() *  ( i + 1 ) );
        int tmp = permutations[ i ];
        permutations[ i ] = permutations[ target ];
        permutations[ target ] = tmp;
    }
}