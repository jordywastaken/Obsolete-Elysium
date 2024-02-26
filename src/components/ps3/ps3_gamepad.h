
#pragma once

enum GamePadButton
{
    BUTTON_L2 = 0,
    BUTTON_R2 = 1,
    BUTTON_L1 = 2,
    BUTTON_R1 = 3,
    BUTTON_TRIANGLE = 4,
    BUTTON_CIRCLE = 5,
    BUTTON_CROSS = 6,
    BUTTON_SQUARE = 7,
    BUTTON_SELECT = 16,
    BUTTON_L3 = 17,
    BUTTON_R3 = 18,
    BUTTON_START = 19,
    BUTTON_DPAD_UP = 20,
    BUTTON_DPAD_RIGHT = 21,
    BUTTON_DPAD_DOWN = 22,
    BUTTON_DPAD_LEFT = 23,
    BUTTON_PS = 24,
    BUTTON_COUNT = 32
};

enum GamePadAnalog
{
    ANALOG_LEFT_X = 0,
    ANALOG_LEFT_Y = 1,
    ANALOG_RIGHT_X = 2,
    ANALOG_RIGHT_Y = 3
};

bool IsButtonDown(int button);
float GetAnalogAxis(int axis);

namespace ps3_gamepad
{
    void start();
}