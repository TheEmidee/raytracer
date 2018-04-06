#pragma once

#include <array> 

struct Vec3;

class PerlinNoise
{
public:

    PerlinNoise( float scale_ );

    float GetNoise( const Vec3 & position ) const;

private:

    void GeneratePermutations( std::array< int, 256 > & permutations ) const;
    void Permute( std::array< int, 256 > & permutations ) const;

    float scale;
    std::array< float, 256 > data;
    std::array< int, 256 > permX;
    std::array< int, 256 > permY;
    std::array< int, 256 > permZ;
};