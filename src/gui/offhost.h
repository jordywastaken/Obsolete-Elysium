
#pragma once

#include "globals.h"

class OffhostMenu
{
public:
    OffhostMenu() = default;

    bool IsOpened();

    void Run();
    void HandleInput();
    void InputSleep(int ms);
    bool IsInputReady();

    void OnOpen();
    void OnClose();
    void OnCancel();
    void OnScrollUp();
    void OnScrollDown();

    vec2_t GetMenuPosition();
    vec2_t GetCurrentOptionPosition();
    float GetMenuHeight();

    void DrawWatermark();
    void DrawBody();
    void DrawComboBox();

    void Title(const char* text);
    void Option(const char* text, void(*action)() = 0);
    void Toggle(const char* text, bool& value, void(*action)(bool) = 0);
    void Slider(const char* text, float& value, float min, float max, float step, int waitMs, const char* fmt = "%.2f");
    void Slider(const char* text, int& value, int min, int max, int step, int waitMs, const char* fmt = "%d");
    void HueSlider(const char* text, float& hue);
    void HueSlider(const char* text, Color* rgb);
    void HueSlider(const char* text, vec4_t* rgba);
    void ComboBox(const char* text, int& index, const char** items, int itemCount);
    void Submenu(const char* text, void(*submenu)(OffhostMenu&) = 0);

    bool Opened{};
    int InputTick{};
    int CurrentOption{};
    int CurrentOptionDrawn{};
    int TotalOptions{};

    static constexpr int SubmenuCount = 10;
    int SubmenuLevel{ -1 };
    void(*SubmenuHandlers[SubmenuCount])(OffhostMenu&) {};
    int SubmenuSavedOption[SubmenuCount]{};

    bool ComboBoxOpened{};
    vec2_t ComboBoxLocation{};
    const char** ComboBoxItems{};
    int ComboBoxItemCount{};
    int ComboBoxCurrentItem{};
};

extern OffhostMenu* Offhost;