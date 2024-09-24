#include "../include/Pixel.hpp"

Pixel::Pixel() {
    this->_r = 0;
    this->_g = 0;
    this->_b = 0;
}

Pixel::Pixel(short r, short g, short b) {
    this->_r = r;
    this->_g = g;
    this->_b = b;

}

Pixel::Pixel(COLORREF cRef) {
    this->_r = (short)GetRValue(cRef);
    this->_g = (short)GetGValue(cRef);
    this->_b = (short)GetBValue(cRef);
}

short Pixel::r() {
    return this->_r;
}

short Pixel::g() {
    return this->_g;
}

short Pixel::b() {
    return this->_b;
}

std::ostream& operator<<(std::ostream& cout, Pixel& pix) {
    cout << "(" << pix._r << ", " << pix._g << ", " << pix._b << ")";
    return cout;
}

// Gets pixel colour at given coordinate
// X - X coordinate to get pixel colour from
// Y - Y coordinate to get pixel colour from
Pixel Pixel::getPixel(int x, int y) {
    // Gets the whole screen's content
    HDC screen = GetDC(NULL);

    // Gets the pixel colour
    COLORREF c = GetPixel(screen, x, y);

    // Release the Device Content
    ReleaseDC(NULL, screen);

    // Copy data to my Pixel struct
    this->_r = (short)GetRValue(c);
    this->_g = (short)GetGValue(c);
    this->_b = (short)GetBValue(c);

    return *this;
}

// X - Top left X coordinate
// Y - Top left Y coordinate
// Width - Width of frame to copy
// Height - Height of frame to copy
// Add filename if you want to save the image, use .bmp file extension
// Warning - returns a malloced array if returnPixels == true, you must free it
Pixel* Pixel::snip(int xStart, int yStart, int width, int height, const char* filename, bool returnPixels) {
    // Initialized now so it exists incase of errors later
    Pixel* pixels = nullptr;
    if (returnPixels) {
        pixels = new Pixel[width * height];
    }

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
    if (returnPixels) {
        for(int i = 0; i < width * height; i++) {
            pixels[i]._r = rgbPixels[i].rgbRed;
            pixels[i]._g = rgbPixels[i].rgbGreen;
            pixels[i]._b = rgbPixels[i].rgbBlue;
        }
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
bool Pixel::areEqual(Pixel& other, int variance) {
    // Checks pixel in range
    if (!(other._r - variance <= this->_r && this->_r <= other._r + variance))
        return false;

    if (!(other._g - variance <= this->_g && this->_g <= other._g + variance))
        return false;

    if (!(other._b - variance <= this->_b && this->_b <= other._b + variance))
        return false;

    // Pixels are within variance range
    return true;
}

// Pass 2 pixels, and it determines the similarity between both
// Variance - how much off each RGB value can be and still be considered the same
bool Pixel::areEqual(Pixel& lhs, Pixel& rhs, int variance) {
    // Checks pixel in range
    if (!(rhs._r - variance <= lhs._r && lhs._r <= rhs._r + variance))
        return false;

    if (!(rhs._g - variance <= lhs._g && lhs._g <= rhs._g + variance))
        return false;

    if (!(rhs._b - variance <= lhs._b && lhs._b <= rhs._b + variance))
        return false;

    // Pixels are within variance range
    return true;
}

// Allows for testing pixel similarities between pixel arrays of equal size
// Variance - how much off each RGB value can be and still be considered the same
bool Pixel::areEqual(Pixel* lhs, Pixel* rhs, int size, int variance) {
    for (int i = 0; i < size; i++)
        if (!areEqual(lhs[i], rhs[i], variance))
            return false;
    
    // Returns true if all other values are equal
    return true;
}

