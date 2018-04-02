#define NOMINMAX

#include <algorithm>

#include "backbuffer.h"

Backbuffer::Backbuffer( const int width, const int height )
    : width( width ), height( height )
{
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = width * height * 4;
    HDC hdc = CreateCompatibleDC( GetDC( 0 ) );
    bitmap = CreateDIBSection( hdc, &bmi, DIB_RGB_COLORS, ( void** ) &bufferBytes, NULL, 0x0 );
    data = new float[ width * height * 4 ];
    memset( data, 0, width * height * 4 * sizeof( data[ 0 ] ) );
}

void Backbuffer::Draw( HDC dc, int dest_width, int dest_height )
{
    uint32_t* dst = bufferBytes;
    const float* src = data;

    for ( int i = 0; i < width * height; ++i )
    {
        uint32_t r = std::min( ( uint32_t ) ( src[ 0 ] * 255.9f ), 255u );
        uint32_t g = std::min( ( uint32_t ) ( src[ 1 ] * 255.9f ), 255u );
        uint32_t b = std::min( ( uint32_t ) ( src[ 2 ] * 255.9f ), 255u );
        *dst = b | ( g << 8 ) | ( r << 16 );
        src += 4;
        dst += 1;
    }
    HDC srcDC = CreateCompatibleDC( dc );
    SetStretchBltMode( dc, COLORONCOLOR );
    SelectObject( srcDC, bitmap );
    StretchBlt( dc, 0, 0, dest_width, dest_height, srcDC, 0, 0, width, height, SRCCOPY );
    DeleteObject( srcDC );
}