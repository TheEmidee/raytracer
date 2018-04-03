#pragma once

#include <math.h>
#include <assert.h>

struct Vec3
{
    Vec3() : x( 0 ), y( 0 ), z( 0 ) {}
    Vec3( float x_, float y_, float z_ ) : x( x_ ), y( y_ ), z( z_ ) {}
    
    float SquareLength() const { return x*x + y*y + z*z; }
    float Length() const { return sqrtf( x*x + y*y + z*z ); }
    void Normalize() { float k = 1.0f / Length(); x *= k; y *= k; z *= k; }

    Vec3 operator-() const { return Vec3( -x, -y, -z ); }
    Vec3& operator+=( const Vec3 & other ) { x += other.x; y += other.y; z += other.z; return *this; }
    Vec3& operator-=( const Vec3 & other ) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Vec3& operator*=( const Vec3 & other ) { x *= other.x; y *= other.y; z *= other.z; return *this; }
    Vec3& operator/=( const Vec3 & other ) { x /= other.x; y /= other.y; z /= other.z; return *this; }
    Vec3& operator*=( float multiplier ) { x *= multiplier; y *= multiplier; z *= multiplier; return *this; }
    Vec3& operator/=( float divider ) { x /= divider; y /= divider; z /= divider; return *this; }

    static Vec3 One() { return Vec3( 1.0f, 1.0f, 1.0f ); }
    static float Dot( const Vec3 & v1, const Vec3 & v2 ) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
    static Vec3 Cross( const Vec3 & v1, const Vec3 & v2 ) 
    { 
        return Vec3(
            v1.y * v2.z - v1.z * v2.y,
            -v1.x * v2.z - v1.z * v2.x,
            v1.x * v2.y - v1.y * v2.x
        );
    }

    float x, y, z;
};

inline Vec3 operator+( const Vec3& a, const Vec3& b ) { return Vec3( a.x + b.x, a.y + b.y, a.z + b.z ); }
inline Vec3 operator-( const Vec3& a, const Vec3& b ) { return Vec3( a.x - b.x, a.y - b.y, a.z - b.z ); }
inline Vec3 operator*( const Vec3& a, const Vec3& b ) { return Vec3( a.x*b.x, a.y*b.y, a.z*b.z ); }
inline Vec3 operator*( const Vec3& a, float b ) { return Vec3( a.x*b, a.y*b, a.z*b ); }
inline Vec3 operator*( float a, const Vec3& b ) { return Vec3( a*b.x, a*b.y, a*b.z ); }
inline Vec3 operator/( const Vec3& b, float a ) { return Vec3( b.x / a, b.y / a, b.z / a ); }
inline Vec3 operator+( float a, const Vec3& b ) { return Vec3( a + b.x, a + b.y, a + b.z ); }
inline Vec3 operator+( const Vec3& b, float a ) { return Vec3( a + b.x, a + b.y, a + b.z ); }

inline void AssertUnit( const Vec3 & v )
{
    assert( fabsf( v.SquareLength() - 1.0f ) < 0.01f );
}

inline Vec3 Normalize( const Vec3 & v ) 
{
    float k = 1.0f / v.Length(); 
    return Vec3( v.x*k, v.y*k, v.z*k ); 
}

inline Vec3 Reflect( const Vec3 & vector, const Vec3 & normal )
{
    return vector - 2 * Vec3::Dot( vector, normal ) * normal;
}

inline bool Refract( const Vec3 & vector, const Vec3 & normal, float ni_over_nt, Vec3 & refracted_vec3 )
{
    AssertUnit( vector );

    float dt = Vec3::Dot( vector, normal );
    float discriminant = 1.0f - ni_over_nt * ni_over_nt * ( 1.0f - dt * dt );

    if ( discriminant > 0.0f )
    {
        refracted_vec3 = ni_over_nt * ( vector - normal * dt ) - normal * sqrtf( discriminant );
        return true;
    }

    return false;
}