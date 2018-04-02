#include "raytracer.h"
#include "backbuffer.h"

void RayTracer::Process( Backbuffer & back_buffer )
{
    for ( int y = 0; y < back_buffer.GetHeight(); y++ )
    {
        float * data = back_buffer.GetData() + y * back_buffer.GetWidth() * 4;
        
        for ( int x = 0; x < back_buffer.GetWidth(); x++ )
        {
            data[ 0 ] = static_cast< float >( x ) / static_cast< float >( back_buffer.GetWidth() );
            data[ 1 ] = static_cast< float >( y ) / static_cast< float >( back_buffer.GetHeight() );
            data[ 2 ] = 0.2f;
            data += 4;
        }
    }
}