#include "sphere.h"
#include "ray.h"
#include "hitinfos.h"
#include "aabb.h"

Sphere::Sphere( const Vec3 & center_, float radius_, std::shared_ptr< const Material > material_ )
    : Hitable( material_ ), center( center_ ), radius( radius_ ), squaredRadius( radius_ * radius_ ), invRadius( 1.0f / radius_ )
{

}

bool Sphere::Hit( const Ray & ray, float min_time, float max_time, HitInfos & hit_infos ) const
{
    Vec3 origin_to_center = ray.origin - center;
    float b = Vec3::Dot( origin_to_center, ray.direction );
    float c = Vec3::Dot( origin_to_center, origin_to_center ) - squaredRadius;
    float discriminant = b*b - c;

    if ( discriminant > 0 )
    {
        float squared_discriminant = sqrtf( discriminant );

        float time = -b - squared_discriminant;

        if ( time < max_time && time > min_time )
        {
            FillHitInfos( hit_infos, ray, time );
            return true;
        }

        time = -b + squared_discriminant;

        if ( time < max_time && time > min_time )
        {
            FillHitInfos( hit_infos, ray, time );
            return true;
        }
    }

    return false;
}

bool Sphere::GetBoundingBox(aabb & box) const
{
    Vec3 radius_vector(radius, radius, radius);

    box = aabb(center - radius_vector, center + radius_vector);

    return true;
}

void Sphere::FillHitInfos( HitInfos & hit_infos, const Ray & ray, float time ) const
{
    hit_infos.Time = time;
    hit_infos.Point = ray.PointAt( time );
    hit_infos.Normal = ( hit_infos.Point - center ) * invRadius;
    hit_infos.Material = material.get();
}