#pragma once

#include <windows.h>
#include <conio.h>
#include <iostream>

class Pixel
{
private:

public:
    short r, g, b;
    Pixel()
    {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }
    Pixel(short r, short g, short b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    // Gets pixel colour at given coordinate
    // X - X coordinate to get pixel colour from
    // Y - Y coordinate to get pixel colour from
    Pixel getPixel(int x, int y)
    {
        // Gets the whole screen's content
        HDC screen = GetDC(NULL);

        // Gets the pixel colour
        COLORREF c = GetPixel(screen, x, y);

        // Release the Device Content
        ReleaseDC(NULL, screen);

        // Copy data to my Pixel struct
        this->r = (int)GetRValue(c);
        this->g = (int)GetGValue(c);
        this->b = (int)GetBValue(c);

        return *this;
    }

    // Pass a pixel, and it determines the comparison
    // to the most recently checked pixel
    bool areEqual(Pixel other)
    {
        if (this->r == other.r && this->g == other.g && this->b == other.b)
            return true;
        return false;
    }
};

