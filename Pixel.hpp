#pragma once

#include <windows.h>
#include <wingdi.h>
#include <iomanip>
#include <conio.h>
#include <iostream>
#include <fstream>

using namespace std;

class Pixel {
private:
    short r, g, b;
    
public:
    
    Pixel() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }
    Pixel(short r, short g, short b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    Pixel(COLORREF cRef) {
        this->r = (short)GetRValue(cRef);
        this->g = (short)GetGValue(cRef);
        this->b = (short)GetBValue(cRef);
    }

    short getr() {
        return this->r;
    }
    short getg() {
        return this->g;
    }
    short getb() {
        return this->b;
    }

    friend std::ostream& operator<<(std::ostream& cout, Pixel& pix) {
        cout << "R: " << pix.r << ", G: " << pix.g << ", B: " << pix.b;
        return cout;
    }

    // Gets pixel colour at given coordinate
    // X - X coordinate to get pixel colour from
    // Y - Y coordinate to get pixel colour from
    Pixel getPixel(int x, int y) {
        // Gets the whole screen's content
        HDC screen = GetDC(NULL);

        // Gets the pixel colour
        COLORREF c = GetPixel(screen, x, y);

        // Release the Device Content
        ReleaseDC(NULL, screen);

        // Copy data to my Pixel struct
        this->r = (short)GetRValue(c);
        this->g = (short)GetGValue(c);
        this->b = (short)GetBValue(c);

        return *this;
    }

    // X - Top left X coordinate
    // Y - Top left Y coordinate
    // Width - Width of frame to copy
    // Height - Height of frame to copy
    // Add filename if you want to save the image, use .bmp file extension
    // Warning - returns a malloced array, you must free it or you will leak memory
    Pixel* snip(int xStart, int yStart, int width, int height, const char* filename = nullptr) {
        // Initialized now so it exists incase of errors later
        Pixel* pixels = new Pixel[width * height];

        // Gets DC of the screen
        HDC hdcScreen = GetDC(NULL);
        
        // Save image state
        HDC hdcScreenMem = CreateCompatibleDC(hdcScreen);

        // Create a compatible bitmap from the Window DC
        HBITMAP hbmScreenMem = CreateCompatibleBitmap(hdcScreen, width, height);

        // Select the compatible bitmap into the compatible memory DC
        SelectObject(hdcScreenMem, hbmScreenMem);

        // Bit block transfer into the compatible memory DC and retrieve pixel information
        RGBQUAD *rgbPixels = new RGBQUAD[width * height];
        BitBlt(hdcScreenMem, 0, 0, width, height, hdcScreen, xStart, yStart, SRCCOPY);

        // Get the BITMAP from the HBITMAP.
        BITMAP bmpScreen;
        GetObjectW(hbmScreenMem, sizeof(BITMAP), &bmpScreen);

        BITMAPINFO bi;

        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = width;
        bi.bmiHeader.biHeight = height;
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;
        bi.bmiHeader.biSizeImage = 0;
        bi.bmiHeader.biXPelsPerMeter = 0;
        bi.bmiHeader.biYPelsPerMeter = 0;
        bi.bmiHeader.biClrUsed = 0;
        bi.bmiHeader.biClrImportant = 0;

        size_t imageBitmapSize = ((bi.bmiHeader.biBitCount * width + 31) / 32) * 4 * height; // Equation found on BMP wiki, confirmed by MSDN
        char* bitmapAlloc = (char*)GlobalAlloc(GMEM_FIXED, imageBitmapSize);

        // Gets the information from the bitmap
        GetDIBits(hdcScreen, hbmScreenMem, 0, (UINT)bmpScreen.bmHeight, bitmapAlloc, &bi, DIB_RGB_COLORS);
        GetDIBits(hdcScreen, hbmScreenMem, 0, (UINT)bmpScreen.bmHeight, rgbPixels, &bi, DIB_RGB_COLORS);

        // Saves pixels before attempting to open file
        for(int i = 0; i < width * height; i++) {
            pixels[i].r = rgbPixels[i].rgbRed;
            pixels[i].g = rgbPixels[i].rgbGreen;
            pixels[i].b = rgbPixels[i].rgbBlue;
        }
        delete [] rgbPixels;

        // If the user enters a filename, a file will be created
        if (filename != nullptr) {
            
            HANDLE imageFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (imageFile == nullptr) goto afterFile;

            BITMAPFILEHEADER bmfHeader;
            
            bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);    // Offset to where the actual bitmap bits start.
            bmfHeader.bfSize = imageBitmapSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);   // Set file size
            bmfHeader.bfType = 0x4d42;                                             // BM bfType 0x4d42 required

            // Pushes image to .bmp file
            DWORD totalBytesWritten = 0;
            
            WriteFile(imageFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &totalBytesWritten, NULL);
            WriteFile(imageFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &totalBytesWritten, NULL);
            WriteFile(imageFile, (LPSTR)bitmapAlloc, imageBitmapSize, &totalBytesWritten, NULL);

            CloseHandle(imageFile);
        }
        // For dealing with everything after the potential file handling
    afterFile:
        
        // Free all used items
        DeleteObject(hbmScreenMem);
        DeleteObject(hdcScreenMem);
        ReleaseDC(NULL, hdcScreen);

        return pixels;
    }

    // Pass a pixel, and it determines the similarity to the most recently checked pixel
    // Variance - how much off each RGB value can be and still be considered the same
    bool areEqual(Pixel& other, int variance = 0) {
        // Checks pixel in range
        if (!(other.r - variance <= this->r && this->r <= other.r + variance))
            return false;

        if (!(other.g - variance <= this->g && this->g <= other.g + variance))
            return false;

        if (!(other.b - variance <= this->b && this->b <= other.b + variance))
            return false;

        // Pixels are within variance range
        return true;
    }

    // Pass 2 pixels, and it determines the similarity between both
    // Variance - how much off each RGB value can be and still be considered the same
    bool areEqual(Pixel& lhs, Pixel& rhs, int variance = 0) {
        // Checks pixel in range
        if (!(rhs.r - variance <= lhs.r && lhs.r <= rhs.r + variance))
            return false;

        if (!(rhs.g - variance <= lhs.g && lhs.g <= rhs.g + variance))
            return false;

        if (!(rhs.b - variance <= lhs.b && lhs.b <= rhs.b + variance))
            return false;

        // Pixels are within variance range
        return true;
    }

    // Allows for testing pixel similarities between pixel arrays of equal size
    // Variance - how much off each RGB value can be and still be considered the same
    bool areEqual(Pixel* lhs, Pixel* rhs, int size, int variance = 0) {
        for (int i = 0; i < size; i++)
            if (!areEqual(lhs[i], rhs[i]))
                return false;
        
        // Returns true if all other values are equal
        return true;
    }
};

