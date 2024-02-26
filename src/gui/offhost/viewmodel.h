
#pragma once

#include "globals.h"
#include "gui/offhost.h"
#include "components/database/db_registry.h"
#include "utils/imports.h"

namespace submenus
{
    void viewmodel(OffhostMenu& menu)
    {
        menu.Title("View model");
        menu.Toggle("Wireframe", Config->ViewModelWireframe);
        menu.Slider("Wire thickness", Config->ViewModelWireSize, 0, 100, 1, 20);
        menu.Toggle("Override camouflage", Config->SpoofCamo);
        menu.Slider("Camouflage index", Config->SpoofedCamo, 0, 100, 1, 80);
    }
}