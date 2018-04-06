#include "perlinnoise.h"

#include "vec3.h"
#include "maths.h"

PerlinNoise::PerlinNoise( float scale_ )
    : scale( scale_ )
{
    for ( int i = 0; i < data.size(); i++ )
    {
        //data[ i ] = RandomFloat01( state );
    }

    GeneratePermutations( permX );
    GeneratePermutations( permY );
    GeneratePermutations( permZ );
}

float PerlinNoise::GetNoise( const Vec3 & position ) const
{
    return 0.0f;
}

void PerlinNoise::GeneratePermutations( std::array< int, 256 > & permutations ) const
{
    for ( int i = 0; i < permutations.size(); i++ )
    {
        permutations[ i ] = i;
    }

    Permute( permutations );
}

void PerlinNoise::Permute( std::array< int, 256 > & permutations ) const
{

}