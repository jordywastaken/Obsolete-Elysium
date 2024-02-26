
#pragma once

#include "globals.h"
#include "gui/offhost.h"

namespace submenus
{
    void layout(OffhostMenu& menu)
    {
        menu.Title("Menu layout");
        menu.Slider("Position X", Config->OffhostPosition.x, 0.0, 1280.0, 5.0, 0, "%.0f");
        menu.Slider("Position Y", Config->OffhostPosition.y, 0.0, 720.0, 5.0, 0, "%.0f");
        menu.Slider("Safe area X", Config->SafeArea.x, 0.0, 50.0, 1.0, 0, "%.0f");
        menu.Slider("Safe area Y", Config->SafeArea.y, 0.0, 50.0, 1.0, 0, "%.0f");
        menu.Slider("Width", Config->OffhostWidth, 250.0, 600.0, 5.0, 0, "%.0f");
        menu.Slider("Padding", Config->OffhostPadding, 0.0, 10.0, 0.05, 0, "%.2f");
        menu.Slider("Title size", Config->OffhostTitleSize, 20.0, 50.0, 0.1, 0, "%.1f");
        menu.Slider("Border size", Config->OffhostBorderSize, 1.0, 10.0, 0.1, 0, "%.1f");
        menu.Slider("Option size", Config->OffhostOptionSize, 15.0, 30.0, 0.5, 0, "%.1f");
    }

    void colors(OffhostMenu& menu)
    {
        menu.Title("Menu colors");
        menu.Toggle("Rainbow fade", Config->FadeColors);
        menu.Slider("Fade Speed", Config->FadeSpeed, -5.0, 5.0, 0.05, 0, "%.2f");
        menu.Slider("Fade hue spacing", Config->FadeHueSpacing, -180.0, 180.0, 1, 0, "%.0f");

        menu.HueSlider("Color 1 Hue", Config->ColorPrimaryAccent.h);
        float s1 = Config->ColorPrimaryAccent.s * 100.0;
        float v1 = Config->ColorPrimaryAccent.v * 100.0;
        menu.Slider("Color 1 Saturation", s1, 0.0, 100.0, 1.0, 20, "%.0f%%");
        menu.Slider("Color 1 Value", v1, 0.0, 100.0, 1.0, 20, "%.0f%%");
        Config->ColorPrimaryAccent.s = s1 / 100.0;
        Config->ColorPrimaryAccent.v = v1 / 100.0;

        menu.HueSlider("Color 2 Hue", Config->ColorSecondaryAccent.h);
        float s2 = Config->ColorSecondaryAccent.s * 100.0;
        float v2 = Config->ColorSecondaryAccent.v * 100.0;
        menu.Slider("Color 2 Saturation", s2, 0.0, 100.0, 1.0, 20, "%.0f%%");
        menu.Slider("Color 2 Value", v2, 0.0, 100.0, 1.0, 20, "%.0f%%");
        Config->ColorSecondaryAccent.s = s2 / 100.0;
        Config->ColorSecondaryAccent.v = v2 / 100.0;
    }

    void settings(OffhostMenu& menu)
    {
        menu.Title("Settings");
        menu.Submenu("Menu layout", layout);
        menu.Submenu("Menu colors", colors);
    }
}