
#include "offhost.h"
#include "components/database/db_registry.h"
#include "components/gfx_3d/r_rendercmds.h"
#include "components/ps3/ps3_gamepad.h"
#include "offhost/aimassist.h"
#include "offhost/camera.h"
#include "offhost/viewmodel.h"
#include "offhost/settings.h"

OffhostMenu* Offhost;

bool OffhostMenu::IsOpened()
{
    return Opened || !IsInputReady();
}

void OffhostMenu::Run()
{
    if (Config->FadeColors)
    {
        auto& primary = Config->ColorPrimaryAccent;
        auto& secondary = Config->ColorSecondaryAccent;

        primary.h += Config->FadeSpeed;

        while (primary.h < 0.0)
            primary.h += 360.0;
        while (primary.h > 360.0)
            primary.h -= 360.0;

        secondary.h = primary.h + Config->FadeHueSpacing;

        while (secondary.h < 0.0)
            secondary.h += 360.0;
        while (secondary.h > 360.0)
            secondary.h -= 360.0;
    }
    HSVtoRGB(Config->ColorPrimaryAccent, g_colorPrimary);
    HSVtoRGB(Config->ColorSecondaryAccent, g_colorSecondary);


    HandleInput();
    DrawWatermark();

    if(IsOpened())
    {
        DrawBody();

        if (SubmenuLevel == -1)
        {
            Title("Elysium");
            Submenu("Aim assistance", submenus::aimassist);
            Submenu("Camera", submenus::camera);
            Submenu("View model", submenus::viewmodel);
            Submenu("Settings", submenus::settings);
        }
        else
        {
            SubmenuHandlers[SubmenuLevel](*this);
        }

        if (ComboBoxOpened)
            DrawComboBox();
    }

    TotalOptions = CurrentOptionDrawn;
    CurrentOptionDrawn = 0;
}

void OffhostMenu::HandleInput()
{
    if (!IsInputReady() || ComboBoxOpened)
        return;

    if (!Opened)
    {
        if (IsButtonDown(BUTTON_L1) && IsButtonDown(BUTTON_DPAD_LEFT))
            OnOpen();

        return;
    }

    if (IsButtonDown(BUTTON_CIRCLE))
        OnCancel();
    else if (IsButtonDown(BUTTON_DPAD_UP))
        OnScrollUp();
    else if (IsButtonDown(BUTTON_DPAD_DOWN))
        OnScrollDown();
}

void OffhostMenu::InputSleep(int ms)
{
    InputTick = Sys_Milliseconds() + ms;
}

bool OffhostMenu::IsInputReady()
{
    return Sys_Milliseconds() >= InputTick;
}

void OffhostMenu::OnOpen()
{
    Opened = true;
    InputSleep(100);
}

void OffhostMenu::OnClose()
{
    Opened = false;
    InputSleep(100);
}

void OffhostMenu::OnCancel()
{
    if (SubmenuLevel == -1)
        return OnClose();

    CurrentOption = SubmenuSavedOption[SubmenuLevel];
    SubmenuLevel--;
    InputSleep(200);
}

void OffhostMenu::OnScrollUp()
{
    CurrentOption--;
    if (CurrentOption < 0)
        CurrentOption = TotalOptions - 1;

    InputSleep(150);
}

void OffhostMenu::OnScrollDown()
{
    CurrentOption++;
    if (CurrentOption > TotalOptions - 1)
        CurrentOption = 0;
    InputSleep(150);
}

vec2_t OffhostMenu::GetMenuPosition()
{
    return { Config->OffhostPosition.x - Config->OffhostWidth * 0.5f, Config->OffhostPosition.y };
}

vec2_t OffhostMenu::GetCurrentOptionPosition()
{
    vec2_t base = GetMenuPosition();
    base.x += Config->OffhostPadding;
    base.y += Config->OffhostTitleSize + Config->OffhostOptionSize * CurrentOptionDrawn;
    return base;
}

float OffhostMenu::GetMenuHeight()
{
    return Config->OffhostTitleSize + Config->OffhostPadding + Config->OffhostOptionSize * TotalOptions;
}

void OffhostMenu::DrawWatermark()
{
    const float textHeight = 17.0;
    const float paddingX = 4.0;
    const float paddingY = 1.0;
    const float border = 2.0;

    vec2_t base = Config->SafeArea;

    const char* watermark = "Elysium";
    float width = GetTextWidth(watermark, textHeight, boldFont) + paddingX * 2.0;
    float height = textHeight + paddingY * 2.0;

    DrawRectangle(width, height, base.x + border, base.y + border, { 0.0, 0.0, 0.0 }, 0.8, 0.0, white);
    DrawRectangle(border, height + border * 2.0, base.x, base.y, g_colorSecondary, 1.0, 0.0, white);
    DrawRectangle(border, height + border * 2.0, base.x + border + width, base.y, g_colorPrimary, 1.0, 0.0, white);
    DrawGradientHorizontal(width, border, base.x + border, base.y, g_colorSecondary, 1.0, g_colorPrimary, 1.0);
    DrawGradientHorizontal(width, border, base.x + border, base.y + border + height, g_colorSecondary, 1.0, g_colorPrimary, 1.0);

    DrawText(watermark, textHeight, base.x + border + paddingX, base.y + border + paddingY, { 1.0, 1.0, 1.0 }, 1.0, 0.0, boldFont);
}

void OffhostMenu::DrawBody()
{
    vec2_t base = GetMenuPosition();
    float width = Config->OffhostWidth;
    float height = GetMenuHeight();
    float border = Config->OffhostBorderSize;

    DrawRectangle(width, height, base.x, base.y, { 0.0, 0.0, 0.0 }, 0.8, 0.0, white);

    DrawRectangle(border, height + border * 2.0, base.x - border, base.y - border, g_colorPrimary, 1.0, 0.0, white);
    DrawRectangle(border, height + border * 2.0, base.x + width, base.y - border, g_colorSecondary, 1.0, 0.0, white);
    DrawGradientHorizontal(width, border, base.x, base.y - border, g_colorPrimary, 1.0, g_colorSecondary, 1.0);
    DrawGradientHorizontal(width, border, base.x, base.y + height, g_colorPrimary, 1.0, g_colorSecondary, 1.0);

    float highlightPos = base.y + Config->OffhostTitleSize + Config->OffhostOptionSize * CurrentOption;
    DrawGradientHorizontal(width - Config->OffhostPadding * 2.0, Config->OffhostOptionSize, base.x + Config->OffhostPadding, highlightPos, g_colorPrimary, 0.5, g_colorSecondary, 0.5);
}

void OffhostMenu::DrawComboBox()
{
    vec2_t base = ComboBoxLocation;
    float comboBoxWidth = Config->OffhostWidth * 0.3;
    float comboBoxItemHeight = Config->OffhostOptionSize - 4.0;
    float comboBoxHeight = comboBoxItemHeight * ComboBoxItemCount;

    DrawRectangle(comboBoxWidth, 1.0, base.x, base.y, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);
    DrawRectangle(comboBoxWidth, 1.0, base.x, base.y + comboBoxHeight - 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);
    DrawRectangle(1.0, comboBoxHeight - 2.0, base.x, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);
    DrawRectangle(1.0, comboBoxHeight - 2.0, base.x + comboBoxWidth - 1.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);
    DrawRectangle(comboBoxWidth - 2.0, comboBoxHeight - 2.0, base.x + 1.0, base.y + 1.0, { 0.0, 0.0, 0.0 }, 0.8, 0.0, white);
    
    for (int i = 0; i < ComboBoxItemCount; i++)
    {
        if (i == ComboBoxCurrentItem)
        {
            DrawGradientHorizontal(comboBoxWidth - 2.0, comboBoxItemHeight - 2.0, base.x + 1.0, base.y + comboBoxItemHeight * i + 1.0, g_colorPrimary, 0.5, g_colorSecondary, 0.5);
        }

        DrawTextCentered(ComboBoxItems[i], comboBoxItemHeight, base.x + comboBoxWidth * 0.5, base.y + comboBoxItemHeight * i, { 1.0, 1.0, 1.0 }, 1.0, normalFont);
    }
}

void OffhostMenu::Title(const char* text)
{
    vec2_t base = GetMenuPosition();
    float width = Config->OffhostWidth;

    DrawTextCentered(text, Config->OffhostTitleSize * 0.85, base.x + width * 0.5, base.y + Config->OffhostTitleSize * 0.05, { 1.0, 1.0, 1.0 }, 1.0, boldFont);
}

void OffhostMenu::Option(const char* text, void(*action)())
{
    vec2_t base = GetCurrentOptionPosition();

    DrawText(text, Config->OffhostOptionSize, base.x + 3.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, normalFont);

    if (IsInputReady() && CurrentOption == CurrentOptionDrawn)
    {
        if (IsButtonDown(BUTTON_CROSS))
        {
            if (action)
                action();

            InputSleep(200);
        }
    }

    CurrentOptionDrawn++;
}

void OffhostMenu::Toggle(const char* text, bool& value, void(*action)(bool))
{
    vec2_t base = GetCurrentOptionPosition();
    float width = Config->OffhostWidth - Config->OffhostPadding * 2.0;
    float toggleSize = Config->OffhostOptionSize - 4.0;

    DrawText(text, Config->OffhostOptionSize, base.x + 3.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, normalFont);

    base.x += width - toggleSize - 2.0;
    base.y += 2.0;

    DrawRectangle(toggleSize, 1.0, base.x, base.y, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);
    DrawRectangle(toggleSize, 1.0, base.x, base.y + toggleSize - 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);
    DrawRectangle(1.0, toggleSize - 2.0, base.x, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);
    DrawRectangle(1.0, toggleSize - 2.0, base.x + toggleSize - 1.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);
    DrawRectangle(toggleSize - 2.0, toggleSize - 2.0, base.x + 1.0, base.y + 1.0, {0.0, 0.0, 0.0}, 0.8, 0.0, white);

    if (value)
    {
        DrawRectangle(toggleSize - 4.0, toggleSize - 4.0, base.x + 2.0, base.y + 2.0, g_colorPrimary, 0.75, 0.0, white);
        DrawRectangle(toggleSize - 8.0, toggleSize - 8.0, base.x + 4.0, base.y + 4.0, g_colorPrimary, 0.75, 0.0, white);
    }

    if (IsInputReady() && CurrentOption == CurrentOptionDrawn)
    {
        if (IsButtonDown(BUTTON_CROSS))
        {
            value ^= 1;

            if (action)
                action(value);

            InputSleep(200);
        }
    }

    CurrentOptionDrawn++;
}

void OffhostMenu::Slider(const char* text, float& value, float min, float max, float step, int waitMs, const char* fmt)
{
    vec2_t base = GetCurrentOptionPosition();
    float width = Config->OffhostWidth - Config->OffhostPadding * 2.0;
    float sliderWidth = Config->OffhostWidth * 0.3;

    DrawText(text, Config->OffhostOptionSize, base.x + 3.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, normalFont);
    base.x += width - sliderWidth - 4.0;
    
    char buffer[16];
    snprintf(buffer, 16, fmt, value);
    DrawTextRight(buffer, Config->OffhostOptionSize, base.x - 4.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, normalFont);
    base.y += Config->OffhostOptionSize * 0.5 - 1.0;

    float fraction = (value - min) / (max - min);
    DrawRectangle(sliderWidth, 2.0, base.x, base.y, { 0.2, 0.2, 0.2 }, 1.0, 0.0, white);
    DrawRectangle(sliderWidth * fraction, 2.0, base.x, base.y, g_colorPrimary, 1.0, 0.0, white);
    DrawRectangle(2.0, 8.0, base.x + sliderWidth * fraction - 1.0, base.y - 3.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);

    if (IsInputReady() && CurrentOption == CurrentOptionDrawn)
    {
        if (IsButtonDown(BUTTON_DPAD_LEFT) || IsButtonDown(BUTTON_DPAD_RIGHT))
        {
            value += IsButtonDown(BUTTON_DPAD_LEFT) ? -step : step;
            InputSleep(waitMs);
        }

        value = Clamp(value, min, max);
    }

    CurrentOptionDrawn++;
}

void OffhostMenu::Slider(const char* text, int& value, int min, int max, int step, int waitMs, const char* fmt)
{
    vec2_t base = GetCurrentOptionPosition();
    float width = Config->OffhostWidth - Config->OffhostPadding * 2.0;
    float sliderWidth = Config->OffhostWidth * 0.3;

    DrawText(text, Config->OffhostOptionSize, base.x + 3.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, normalFont);
    base.x += width - sliderWidth - 4.0;

    char buffer[16];
    snprintf(buffer, 16, fmt, value);
    DrawTextRight(buffer, Config->OffhostOptionSize, base.x - 4.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, normalFont);
    base.y += Config->OffhostOptionSize * 0.5 - 1.0;

    float fraction = static_cast<float>(value - min) / static_cast<float>(max - min);
    DrawRectangle(sliderWidth, 2.0, base.x, base.y, { 0.2, 0.2, 0.2 }, 1.0, 0.0, white);
    DrawRectangle(sliderWidth * fraction, 2.0, base.x, base.y, g_colorPrimary, 1.0, 0.0, white);
    DrawRectangle(2.0, 8.0, base.x + sliderWidth * fraction - 1.0, base.y - 3.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);

    if (IsInputReady() && CurrentOption == CurrentOptionDrawn)
    {
        if (IsButtonDown(BUTTON_DPAD_LEFT) || IsButtonDown(BUTTON_DPAD_RIGHT))
        {
            value += IsButtonDown(BUTTON_DPAD_LEFT) ? -step : step;
            InputSleep(waitMs);
        }

        value = Clamp(value, min, max);
    }

    CurrentOptionDrawn++;
}

void OffhostMenu::HueSlider(const char* text, float& hue)
{
    vec2_t base = GetCurrentOptionPosition();
    float width = Config->OffhostWidth - Config->OffhostPadding * 2.0;
    float sliderWidth = Config->OffhostWidth * 0.3;

    DrawText(text, Config->OffhostOptionSize, base.x + 3.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, normalFont);
    base.x += width - sliderWidth - 4.0;

    char buffer[16];
    snprintf(buffer, 16, "%.0f\xB0", hue);
    DrawTextRight(buffer, Config->OffhostOptionSize, base.x - 4.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, normalFont);
    base.y += Config->OffhostOptionSize * 0.5 - 1.0;

    Color hueTable[7]
    {
        { 1.0, 0.0, 0.0 }, // Red
        { 1.0, 1.0, 0.0 }, // Yellow
        { 0.0, 1.0, 0.0 }, // Green
        { 0.0, 1.0, 1.0 }, // Cyan
        { 0.0, 0.0, 1.0 }, // Blue
        { 1.0, 0.0, 1.0 }, // Pink
        { 1.0, 0.0, 0.0 }  // Red
    };

    for (int i = 0; i < 6; i++)
    {
        DrawGradientHorizontal(sliderWidth / 6.0, 2.0, base.x + i * sliderWidth / 6.0, base.y, hueTable[i], 1.0, hueTable[i + 1], 1.0);
    }

    float fraction = hue / 360.0;
    DrawRectangle(2.0, 8.0, base.x + sliderWidth * fraction - 1.0, base.y - 3.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);

    if (IsInputReady() && CurrentOption == CurrentOptionDrawn)
    {
        if (IsButtonDown(BUTTON_DPAD_LEFT) || IsButtonDown(BUTTON_DPAD_RIGHT))
        {
            hue += IsButtonDown(BUTTON_DPAD_LEFT) ? -1.0 : 1.0;
        }

        while (hue > 360.0)
            hue -= 360.0;
        while (hue < 0.0)
            hue += 360.0;
    }

    CurrentOptionDrawn++;
}

void OffhostMenu::HueSlider(const char* text, Color* rgb)
{
    Color hsv;
    RGBtoHSV(*rgb, hsv);
    HueSlider(text, hsv.h);
    HSVtoRGB(hsv, *rgb);
}

void OffhostMenu::HueSlider(const char* text, vec4_t* rgba)
{
    Color rgb = { rgba->r, rgba->g, rgba->b };
    Color hsv;

    RGBtoHSV(rgb, hsv);
    HueSlider(text, hsv.h);
    HSVtoRGB(hsv, rgb);

    rgba->r = rgb.r;
    rgba->g = rgb.g;
    rgba->b = rgb.b;
}

void OffhostMenu::ComboBox(const char* text, int& index, const char** items, int itemCount)
{
    vec2_t base = GetCurrentOptionPosition();
    float width = Config->OffhostWidth - Config->OffhostPadding * 2.0;
    float comboBoxWidth = Config->OffhostWidth * 0.3;
    float comboBoxHeight = Config->OffhostOptionSize - 4.0;

    DrawText(text, Config->OffhostOptionSize, base.x + 3.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, normalFont);
    base.x += width - comboBoxWidth - 4.0;
    base.y += 2.0;

    DrawRectangle(comboBoxWidth, 1.0, base.x, base.y, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);
    DrawRectangle(comboBoxWidth, 1.0, base.x, base.y + comboBoxHeight - 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);
    DrawRectangle(1.0, comboBoxHeight - 2.0, base.x, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);
    DrawRectangle(1.0, comboBoxHeight - 2.0, base.x + comboBoxWidth - 1.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, white);
    DrawRectangle(comboBoxWidth - 2.0, comboBoxHeight - 2.0, base.x + 1.0, base.y + 1.0, { 0.0, 0.0, 0.0 }, 0.8, 0.0, white);

    DrawTextCentered(items[index], comboBoxHeight, base.x + comboBoxWidth * 0.5, base.y, { 1.0, 1.0, 1.0 }, 1.0, normalFont);

    if (IsInputReady() && CurrentOption == CurrentOptionDrawn)
    {
        if (IsButtonDown(BUTTON_CROSS))
        {
            if (ComboBoxOpened)
            {
                ComboBoxOpened = false;
                index = ComboBoxCurrentItem;
            }
            else
            {
                ComboBoxOpened = true;
                ComboBoxLocation = base;
                ComboBoxItems = items;
                ComboBoxItemCount = itemCount;
                ComboBoxCurrentItem = index;
            }

            InputSleep(200);
        }

        if (ComboBoxOpened)
        {
            if (IsButtonDown(BUTTON_CIRCLE))
            {
                ComboBoxOpened = false;
                InputSleep(200);
            }
            else if (IsButtonDown(BUTTON_DPAD_UP))
            {
                ComboBoxCurrentItem--;
                if (ComboBoxCurrentItem < 0)
                    ComboBoxCurrentItem = ComboBoxItemCount - 1;

                InputSleep(150);
            }
            else if (IsButtonDown(BUTTON_DPAD_DOWN))
            {
                ComboBoxCurrentItem++;
                if (ComboBoxCurrentItem > ComboBoxItemCount - 1)
                    ComboBoxCurrentItem = 0;

                InputSleep(150);
            }
        }
    }

    CurrentOptionDrawn++;
}

void OffhostMenu::Submenu(const char* text, void(*submenu)(OffhostMenu&))
{
    vec2_t base = GetCurrentOptionPosition();
    float width = Config->OffhostWidth - Config->OffhostPadding * 2.0;

    DrawText(text, Config->OffhostOptionSize, base.x + 3.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, 0.0, normalFont);
    DrawTextRight("->", Config->OffhostOptionSize, base.x + width - 3.0, base.y + 1.0, { 1.0, 1.0, 1.0 }, 1.0, normalFont);

    if (IsInputReady() && CurrentOption == CurrentOptionDrawn)
    {
        if (IsButtonDown(BUTTON_CROSS))
        {
            if (submenu)
            {
                SubmenuLevel++;
                SubmenuSavedOption[SubmenuLevel] = CurrentOption;
                SubmenuHandlers[SubmenuLevel] = submenu;
                CurrentOption = 0;
            }

            InputSleep(200);
        }
    }

    CurrentOptionDrawn++;
}