#pragma once

#include <math.h>

struct Vec3
{
    Vec3() : x( 0 ), y( 0 ), z( 0 ) {}
    Vec3( float x, float y, float z ) : x( x ), y( y ), z( z ) {}
    
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