#include "raytracer.h"
#include "backbuffer.h"
#include "vec3.h"

void RayTracer::Process( Backbuffer & back_buffer )
{
    for ( int y = 0; y < back_buffer.GetHeight(); y++ )
    {
        float * data = back_buffer.GetData() + y * back_buffer.GetWidth() * 4;
        
        for ( int x = 0; x < back_buffer.GetWidth(); x++ )
        {
            Vec3 color;
            color.x = static_cast< float >( x ) / static_cast< float >( back_buffer.GetWidth() );
            color.y = static_cast< float >( y ) / static_cast< float >( back_buffer.GetHeight() );
            color.z = 0.2f;

            data[ 0 ] = color.x;
            data[ 1 ] = color.y;
            data[ 2 ] = color.z;
            data += 4;
        }
    }
}