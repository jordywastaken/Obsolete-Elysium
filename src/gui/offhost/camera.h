
#pragma once

#include "globals.h"
#include "gui/offhost.h"

namespace submenus
{
    void camera(OffhostMenu& menu)
    {
        menu.Title("Camera");
        menu.Slider("Field of view", Config->Fov, 40.0, 160.0, 1.0, 0, "%.0f\xB0");
    }
}