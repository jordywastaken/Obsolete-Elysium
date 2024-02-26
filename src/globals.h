
#pragma once

#include "game/game.h"
#include "components/gfx_3d/r_rendercmds.h"

#define ELYSIUM_VERSION_MAJOR 1
#define ELYSIUM_VERSION_MINOR 0

extern Color g_colorPrimary;
extern Color g_colorSecondary;

class Configuration
{
public:
    Configuration() = default;

    bool AimbotEnabled{ false };
    int AimbotVisibility{ 2 };
    int AimType{ 1 };
    int AimBoneIndex{ 1 };

    bool NoRecoil{ true };
    bool NoSpread{ true };
    bool NoKick{ false };
    bool NoSway{ true };
    bool NoBob{ false };
    bool NoLean{ false };
    bool NoFlinch{ false };

    bool AutoShoot{ true };
    bool FireMacro{ true };
    bool JumpMacro{ true };

    float Fov{ 65.0 };

    bool ViewModelWireframe{ false };
    int ViewModelWireSize{ 8 };
    bool SpoofCamo{ true };
    int SpoofedCamo{ 17 };
    bool SynchronizeDarkMatterColors{ false };

    vec2_t SafeArea{ 30.0, 15.0 };

    vec2_t OffhostPosition{ 640.0, 100.0 };
    float OffhostWidth{ 400.0 };
    float OffhostTitleSize{ 34.0 };
    float OffhostBorderSize{ 3.0 };
    float OffhostOptionSize{ 22.0 };
    float OffhostPadding{ 4.0 };

    Color ColorPrimaryAccent{ 0.0, 0.93, 1.0 };
    Color ColorSecondaryAccent{ 50.0, 0.93, 1.0 };
    bool FadeColors{ true };
    float FadeSpeed{ 1.0 };
    float FadeHueSpacing{ 50.0 };
};

extern Configuration* Config;