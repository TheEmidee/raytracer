#include "sphere.h"
#include "ray.h"
#include "hitinfos.h"

Sphere::Sphere( const Vec3 & center_, float radius_, std::shared_ptr< const Material > material_ )
    : Hitable( material_ ), center( center_ ), radius( radius_ )
{

}

bool Sphere::Hit( const Ray & ray, float min_time, float max_time, HitInfos & hit_infos ) const
{
    Vec3 origin_to_center = ray.origin - center;
    //float a = Vec3::Dot( ray.direction, ray.direction );
    float b = Vec3::Dot( origin_to_center, ray.direction );
    float c = Vec3::Dot( origin_to_center, origin_to_center ) - radius * radius;
    float discriminant = b*b - /*a**/c;

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

void Sphere::FillHitInfos( HitInfos & hit_infos, const Ray & ray, float time ) const
{
    hit_infos.Time = time;
    hit_infos.Point = ray.PointAt( time );
    hit_infos.Normal = ( hit_infos.Point - center ) / radius;
    hit_infos.Material = material;
}