#pragma once

#include <windows.h>
#include <iostream>
#include <fstream>

class Pixel {
private:
    short _r, _g, _b;
    
public:
    
    Pixel();
    Pixel(short r, short g, short b);
    Pixel(COLORREF cRef);

    short r();
    short g();
    short b();

    // Gets pixel colour at given coordinate
    // X - X coordinate to get pixel colour from
    // Y - Y coordinate to get pixel colour from
    Pixel getPixel(int x, int y);

    // X - Top left X coordinate
    // Y - Top left Y coordinate
    // Width - Width of frame to copy
    // Height - Height of frame to copy
    // Add filename if you want to save the image, use .bmp file extension
    // Warning - returns a malloced array if returnPixels == true, you must free it
    Pixel* snip(int xStart, int yStart, int width, int height, const char* filename = nullptr, bool returnPixels = true);

    // Pass a pixel, and it determines the similarity to the most recently checked pixel
    // Variance - how much off each RGB value can be and still be considered the same
    bool areEqual(Pixel& other, int variance = 0);

    // Pass 2 pixels, and it determines the similarity between both
    // Variance - how much off each RGB value can be and still be considered the same
    bool areEqual(Pixel& lhs, Pixel& rhs, int variance = 0);

    // Allows for testing pixel similarities between pixel arrays of equal size
    // Variance - how much off each RGB value can be and still be considered the same
    bool areEqual(Pixel* lhs, Pixel* rhs, int size, int variance = 0);

    friend std::ostream& operator<<(std::ostream& cout, Pixel& pixel);
};

