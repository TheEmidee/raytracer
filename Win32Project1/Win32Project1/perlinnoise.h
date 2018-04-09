#pragma once

#include <vector> 

#include "vec3.h"

class PerlinNoise
{
public:

    PerlinNoise( int resolution_ );

    float GetNoise( const Vec3 & position ) const;
    float GetTurbulence( const Vec3 & position, int depth = 7 ) const;

private:

    void GeneratePermutations( std::vector< int > & permutations ) const;
    void Permute( std::vector< int > & permutations ) const;

    std::vector< Vec3 > data;
    std::vector< int > permX;
    std::vector< int > permY;
    std::vector< int > permZ;
};