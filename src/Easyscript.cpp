#include "../include/Easyscript.hpp"

Easyscript::Easyscript(double pause) {
    this->PAUSE = pause;
}

void Easyscript::rest() {
    if (this->PAUSE <= 0)
        return;

    Sleep(this->PAUSE * MIL_TO_SEC);
}

void Easyscript::setPause(double val) {
    if (val < 0)
        return;
    this->PAUSE = val;
}

double Easyscript::getPause() {
    return this->PAUSE;
}

POINT Easyscript::getMousePos() {
    POINT cPos;
    int x = GetCursorPos(&cPos);
    rest();
    return cPos;
}

void Easyscript::sleep(double time) {
    if (time <= 0)
        return;
    Sleep(time * MIL_TO_SEC);
}

void Easyscript::click(int x, int y, int clicks, double timeBetweenClicks) {
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

void Easyscript::scroll(int scrollDirection, int scrolls, double timeBetweenScrolls) {
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

void Easyscript::keyInput(int key, int pressType, double holdKey) {
    // Create and setup input struct with constant data
    INPUT in = {0};
    in.type = INPUT_KEYBOARD;
    in.ki.dwExtraInfo = 0;
    in.ki.time = 0;
    in.ki.wScan = key;
    in.ki.wVk = 0;

    // Does key down command
    if (pressType == ES_KEY_ACTION_PRESS || pressType == ES_KEY_ACTION_DUAL) {
        in.ki.dwFlags = KEYEVENTF_SCANCODE;
        SendInput(1, &in, sizeof(INPUT));
    }
    // Adds sleep between keys
    if (pressType == ES_KEY_ACTION_DUAL) {
        this->sleep(holdKey);
    }
    // Does key release command
    if (pressType == ES_KEY_ACTION_RELEASE || pressType == ES_KEY_ACTION_DUAL) {
        in.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        SendInput(1, &in, sizeof(INPUT));
    }
    rest();
}

void Easyscript::move(int x, int y) {
    // Point object incase either of the values are defaulted
    POINT p{};
    GetCursorPos(&p);
    if (x == None) {
        x = p.x;
    }

    if (y == None) {
        y = p.y;
    }

    SetCursorPos(x, y);
    rest();
}

Easyscript::~Easyscript() {
    // Nothing todo
}

