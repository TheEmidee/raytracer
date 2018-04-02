#pragma once

#include <windows.h>
#include <stdint.h>

class Backbuffer
{
public:

    Backbuffer( const int width, const int height );

    inline int GetWidth() const { return width; }
    inline int GetHeight() const { return height; }
    inline float * GetData() { return data; }

    void Draw( HDC dc, int dest_width, int dest_height );

private:

    int width;
    int height;
    float * data;
    uint32_t* bufferBytes;
    HBITMAP bitmap;
};