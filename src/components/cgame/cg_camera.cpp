
#include "cg_camera.h"
#include "globals.h"
#include "components/scheduler.h"
#include "components/ps3/ps3_gamepad.h"
#include "game/game.h"
#include "utils/hooking.h"

float ConvertToFovDx(float fov)
{
    // expected result = 14.64
    // 1.  atan((14.0 / (x * 2.0 * (x / (12700.0 - x) + 1.0))) * (4 / 3)) * 114.591552 = 65
    // 2.  atan((14.0 / (x * 2.0 * (x / (12700.0 - x) + 1.0))) * (4 / 3)) = 65 / 114.591552 = 0.567031107
    // 3.  (14.0 / (x * 2.0 * (x / (12700.0 - x) + 1.0))) * (4 / 3) = tan(0.567031107) = 0.63678786053
    // 4.  14.0 / (x * 2.0 * (x / (12700.0 - x) + 1.0)) = 0.63678786053 * (3 / 4) = 0.4775908954
    // 5.  x * 2.0 * (x / (12700.0 - x) + 1.0) = 14 / 0.4775908954 = 29.3137916465
    // 6.  x * (x / (12700.0 - x) + 1) = 29.3137916465 / 2 = 14.6568958233 that's close enough...

    float x = tanf(fov / 114.591552) * (3.0 / 4.0);
    return 7.0 / x;
}

bool valuesSaved = false;
vec3_t currentOrigin;
vec3_t currentAngles;

void CG_DrawActiveFrame_Hook(CallContext_t& ctx)
{
    // .text:0x1337 60 00 00 00 nop                     # No Operation

    if (!valuesSaved)
    {
        currentOrigin = cg->refdef.vieworg;
        currentAngles = cg->refdefViewAngles;
        valuesSaved = true;
    }

    vec3_t forward, right;
    AngleVectors(&currentAngles, &forward, &right, 0);

    float movementSpeed = IsButtonDown(BUTTON_L3) ? 15.0 : 7.5;
    float cameraSpeed = 3.0;

    float forwardMove = -GetAnalogAxis(ANALOG_LEFT_Y) * movementSpeed;
    float rightMove = GetAnalogAxis(ANALOG_LEFT_X) * movementSpeed;

    float pitchMove = GetAnalogAxis(ANALOG_RIGHT_Y) * cameraSpeed;
    float yawMove = -GetAnalogAxis(ANALOG_RIGHT_X) * cameraSpeed;

    currentOrigin.x += forward.x * forwardMove + right.x * rightMove;
    currentOrigin.y += forward.y * forwardMove + right.y * rightMove;
    currentOrigin.z += forward.z * forwardMove + right.z * rightMove;

    currentAngles.x += pitchMove;
    currentAngles.y += yawMove;

    cg->refdef.vieworg = currentOrigin;
    cg->refdefViewAngles = currentAngles;
    AnglesToAxis(&currentAngles, cg->refdef.viewaxis);
}

namespace cg_camera
{
    void start()
    {
        scheduler::loop(scheduler::renderer, []
            {
                if (cg_875C84B7)
                    cg_875C84B7->current.value = ConvertToFovDx(Config->Fov);
            }, 0);

        //hook::inject(0x1337, CG_DrawActiveFrame_Hook);
    }
}