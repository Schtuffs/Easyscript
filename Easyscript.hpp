#pragma once

#include <windows.h>
#include <winuser.h>
#include <conio.h>
#include <iostream>
#include "Pixel.hpp"

#define MIL_TO_SEC  1000
#define None        0x7FFFFFFF
#define SCROLL_UP   1
#define SCROLL_DOWN -1

class Easyscript {
private:
    // PAUSE - amount of time to pause after each Easyscript call
    // DURATION - amount of time to execute task over
    double PAUSE, DURATION;

    // Rests for the PAUSE time between
    void rest() {
        if (this->PAUSE == 0)
            return;

        Sleep(this->PAUSE * MIL_TO_SEC);
    }

public:
    // For extra pixel related functions
    Pixel pixel;

    Easyscript() {
        this->PAUSE = 0.1;
        this->DURATION = 0.1;
    }
    ~Easyscript() = default;
    Easyscript(const Easyscript&) = delete; // Delete if they try to use '=' initializer

    // These functions are for variable setting

    // Val - new value for the pause to be set to
    void setPause(double val) {
        if (val < 0)
            return;
        this->PAUSE = val;
    }

    // Get pause value
    double getPause() {
        return this->PAUSE;
    }

    // Val - new value for the duration to be set to
    void setDuration(double val) {
        if (val < 0)
            return;
        this->DURATION = val;
    }

    // Returns the mouse position with POINT struct
    POINT getMousePos() {
        POINT cPos;
        int x = GetCursorPos(&cPos);
        rest();
        return cPos;
    }

    // Time - amount of time to sleep in seconds
    void sleep(double time) {
        if (time == 0)
            return;
        Sleep(time * MIL_TO_SEC);
    }

    // X - X coordinate of click
    // Y - Y coordinate of click
    // Clicks - how many clicks will be performed
    // timeBetweenClicks - how much time will elapse between clicks
    void click(int x = None, int y = None, int clicks = 1, double timeBetweenClicks = 0.1) {
        // Catch people who try to put negative time between clicks
        if (timeBetweenClicks < 0)
            timeBetweenClicks = 0;

        // Catch people who try to put negative clicks
        if (clicks < 0)
            return;

        // Only move if a real mouse pos is given
        if (x != None || y != None)
            this->move(x, y);

        // Input struct
        INPUT in[2];

        // Flags for mouse down and up
        in[0].type = INPUT_MOUSE;
        in[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        in[1].type = INPUT_MOUSE;
        in[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

        // Send the click
        SendInput(2, in, sizeof(INPUT));

        // Main loop for going through mouse input, extra click already accounted for
        for (int i = 1; i < clicks; i++) {
            this->sleep(timeBetweenClicks);
            SendInput(2, in, sizeof(INPUT));
        }
        rest();
    }

    void scroll(int scrollDirection = SCROLL_DOWN, int scrolls = 1, double timeBetweenScrolls = 0.1) {
        // Ensure a proper scroll direction is given
        if (scrollDirection != SCROLL_UP && scrollDirection != SCROLL_DOWN)
            return;
        
        INPUT in[1];

        // Flags for mouse down and up
        in->type = INPUT_MOUSE;
        in->mi.dwFlags = MOUSEEVENTF_WHEEL;
        in->mi.mouseData = scrollDirection * 100;

        // Send the click
        SendInput(1, in, sizeof(INPUT));

        // Main loop for going through mouse input, extra click already accounted for
        for (int i = 1; i < scrolls; i++) {
            this->sleep(timeBetweenScrolls);
            SendInput(1, in, sizeof(INPUT));
        }
        rest();
    }

    // keys - a string that will get types automatically
    // timeBetweenKeys - time in seconds between each key input
    // this function will NOT use special characters
    // use "specialKeyInput()" for special keys
    void keyInput(string keys, double timeBetweenKeys = 0.1) {
        INPUT in[2];

        in[0].type = INPUT_KEYBOARD;
        in[0].ki.wVk = toupper(keys[0]);
        in[0].ki.dwFlags = KEYEVENTF_EXTENDEDKEY;

        in[1].type = INPUT_KEYBOARD;
        in[1].ki.wVk = toupper(keys[0]);
        in[1].ki.dwFlags = KEYEVENTF_KEYUP;
        
        SendInput(2, in, sizeof(INPUT));

        // Main loop for going through keys in string
        for (int i = 1; i < keys.length(); i++) {
            in[0].type = INPUT_KEYBOARD;
            in[0].ki.wVk = toupper(keys[i]);
            in[0].ki.dwFlags = KEYEVENTF_EXTENDEDKEY;

            in[1].type = INPUT_KEYBOARD;
            in[1].ki.wVk = toupper(keys[i]);
            in[1].ki.dwFlags = KEYEVENTF_KEYUP;
            
            this->sleep(timeBetweenKeys);
            SendInput(2, in, sizeof(INPUT));
        }
        rest();
    }

    // key - a special key, use defined values. EX: VK_CAPITAL, VK_BACK
    void specialKeyInput(int key, int repeatKey = 1, double repeatKeyDelay = 0.1) {
        INPUT in[2];

        for(int i = 0; i < repeatKey; i++) {
            in[0].type = INPUT_KEYBOARD;
            in[0].ki.wVk = key;

            in[1].type = INPUT_KEYBOARD;
            in[1].ki.wVk = key;
            in[1].ki.dwFlags = KEYEVENTF_KEYUP;

            SendInput(2, in, sizeof(INPUT));
            this->sleep(repeatKeyDelay);
        }
        rest();
    }

    // X - X coordinate to move to
    // Y - Y coordinate to move to
    void move(int x, int y) {
        // Point object incase either of the values are defaulted
        POINT p{};
        if (x == None) {
            GetCursorPos(&p);
            x = p.x;
        }

        if (y == None) {
            GetCursorPos(&p);
            y = p.y;
        }

        SetCursorPos(x, y);
        rest();
    }
};

static Easyscript easyscript;

