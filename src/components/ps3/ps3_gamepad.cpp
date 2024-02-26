
#include "ps3_gamepad.h"
#include "game/game.h"
#include "gui/offhost.h"
#include "utils/hooking.h"
#include "utils/imports.h"
#include <cell/pad/libpad.h>

CellPadData padData[7];

bool IsButtonDown(int button)
{
    const int digitals = (padData[0].button[CELL_PAD_BTN_OFFSET_DIGITAL1] << 16) | padData[0].button[CELL_PAD_BTN_OFFSET_DIGITAL2];

    CellPadInfo2 info;
    if (cellPadGetInfo2(&info) != 0 || (info.port_status[0] & 1) == 0)
        return false;

    return (digitals >> button) & 1;
}

void RescaleAnalog(int* x, int* y, int dead)
{
    const float analogX = static_cast<float>(*x);
    const float analogY = static_cast<float>(*y);
    const float deadZone = static_cast<float>(dead);
    const float maximum = 32768.0;

    float magnitude = __builtin_fsqrts(analogX * analogX + analogY * analogY);
    if (magnitude >= deadZone)
    {
        float scalingFactor = maximum / magnitude * (magnitude - deadZone) / (maximum - deadZone);
        *x = (int)(analogX * scalingFactor);
        *y = (int)(analogY * scalingFactor);
    }
    else
    {
        *x = 0;
        *y = 0;
    }
}

void PollLeftAnalog(CellPadData* data, int* x, int* y)
{
    static int hires_x = 0;
    static int hires_y = 0;

    int lx = ((short)data->button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X] - 127) * 256;
    int ly = ((short)data->button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y] - 127) * 256;
    RescaleAnalog(&lx, &ly, 7680);
    hires_x += lx;
    hires_y += ly;
    if (hires_x || hires_y)
    {
        // slow down pointer, could be made user-adjustable
        const int slowdown = 16;

        *x += hires_x / slowdown;
        *y += hires_y / slowdown;

        hires_x %= slowdown;
        hires_y %= slowdown;
    }
}

void PollRightAnalog(CellPadData* data, int* x, int* y)
{
    static int hires_x = 0;
    static int hires_y = 0;

    int rx = ((short)data->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X] - 127) * 256;
    int ry = ((short)data->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y] - 127) * 256;
    RescaleAnalog(&rx, &ry, 7680);
    hires_x += rx;
    hires_y += ry;
    if (hires_x || hires_y)
    {
        // slow down pointer, could be made user-adjustable
        const int slowdown = 16;

        *x += hires_x / slowdown;
        *y += hires_y / slowdown;

        hires_x %= slowdown;
        hires_y %= slowdown;
    }
}

float GetAnalogAxis(int axis)
{
    CellPadInfo2 info;

    if (cellPadGetInfo2(&info) != 0 || (info.port_status[0] & 1) == 0)
        return false;

    int lx = 0;
    int ly = 0;
    PollLeftAnalog(&padData[0], &lx, &ly);

    int rx = 0;
    int ry = 0;
    PollRightAnalog(&padData[0], &rx, &ry);

    switch (axis)
    {
    case(ANALOG_LEFT_X):
        return static_cast<float>(Clamp(lx, -2048, 2048)) / 2048.0;
    case(ANALOG_LEFT_Y):
        return static_cast<float>(Clamp(ly, -2048, 2048)) / 2048.0;
    case(ANALOG_RIGHT_X):
        return static_cast<float>(Clamp(rx, -2048, 2048)) / 2048.0;
    case(ANALOG_RIGHT_Y):
        return static_cast<float>(Clamp(ry, -2048, 2048)) / 2048.0;
    default:
        return 0.0;
    }
}

namespace ps3_gamepad
{
    void start()
    {
        padData[0].button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X] = 128;
        padData[0].button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y] = 128;
        padData[0].button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X] = 128;
        padData[0].button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y] = 128;

        hook::inject(0x1337, [](CallContext_t& ctx)
            {
                auto port = ctx.GetGpr<uint32_t>(3);
                auto data = ctx.GetGpr<CellPadData*>(4);

                ctx.r3 = cellPadGetData(port, &padData[port]);

                _sys_memcpy(data, &padData[port], sizeof(CellPadData));

                if (Offhost->IsOpened())
                {
                    data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~(CELL_PAD_CTRL_UP | CELL_PAD_CTRL_DOWN | CELL_PAD_CTRL_LEFT | CELL_PAD_CTRL_RIGHT);
                    data->button[CELL_PAD_BTN_OFFSET_DIGITAL2] &= ~(CELL_PAD_CTRL_CROSS | CELL_PAD_CTRL_CIRCLE);
                }
            });
    }
}