#pragma once

#include <windows.h>
#include <iostream>

#include "Pixel.hpp"

#define MIL_TO_SEC  1000
#define None        0x7FFFFFFF
#define SCROLL_UP   1
#define SCROLL_DOWN -1

// Keyboard input codes
#define ES_KEY_Q    0x10
#define ES_KEY_W    0x11
#define ES_KEY_E    0x12
#define ES_KEY_R    0x13
#define ES_KEY_T    0x14
#define ES_KEY_Y    0x15
#define ES_KEY_U    0x16
#define ES_KEY_I    0x17
#define ES_KEY_O    0x18
#define ES_KEY_P    0x19

#define ES_KEY_A    0x1e
#define ES_KEY_S    0x1f
#define ES_KEY_D    0x20
#define ES_KEY_F    0x21
#define ES_KEY_G    0x22
#define ES_KEY_H    0x23
#define ES_KEY_J    0x24
#define ES_KEY_K    0x25
#define ES_KEY_L    0x26

#define ES_KEY_Z    0x2d
#define ES_KEY_X    0x2e
#define ES_KEY_C    0x2f
#define ES_KEY_V    0x30
#define ES_KEY_B    0x31
#define ES_KEY_N    0x32
#define ES_KEY_M    0x33

#define ES_KEY_SPACE        0x39
#define ES_KEY_BACKSPACE    0x0e
#define ES_KEY_RETURN       0x1c
#define ES_KEY_EXIT         0x01
#define ES_KEY_DELETE       0x0e
#define ES_KEY_TAB          0x0f

#define ES_KEY_ACTION_PRESS   0x1001
#define ES_KEY_ACTION_RELEASE 0x1002
#define ES_KEY_ACTION_DUAL    0x1003

class Easyscript {
private:
    // PAUSE - amount of time to pause after each Easyscript call
    double PAUSE;

    // Rests for the PAUSE time between
    void rest();

public:
    // For extra pixel related functions
    Pixel pixel;

    Easyscript(double pause = 0.1);
    ~Easyscript();

    // These functions are for variable setting

    // Val - new value for the pause to be set to
    void setPause(double val);

    // Get pause value
    double getPause();

    // Returns the mouse position with POINT struct
    POINT getMousePos();

    // Time - amount of time to sleep in seconds
    void sleep(double time);

    // X - X coordinate of click
    // Y - Y coordinate of click
    // Clicks - how many clicks will be performed
    // timeBetweenClicks - how much time will elapse between clicks
    void click(int x = None, int y = None, int clicks = 1, double timeBetweenClicks = 0.1);

    void scroll(int scrollDirection = SCROLL_DOWN, int scrolls = 1, double timeBetweenScrolls = 0.1);

    // key - key to be pressed
    // pressType - whether the key should be pressed down, released, or both
    // holdKey - how long a key should be held for. Only works if pressType == ES_KEY_ACTION_DUAL
    // this function will NOT use special characters
    // use "specialKeyInput()" for special keys
    void keyInput(int key, int pressType = ES_KEY_ACTION_DUAL, double holdKey = 0.0);

    // X - X coordinate to move to
    // Y - Y coordinate to move to
    void move(int x, int y);
};

