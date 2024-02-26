
#pragma once

#include "game/game.h"

using Color = vec3_t;

void HSVtoRGB(Color hsv, Color& rgb);
void RGBtoHSV(Color rgb, Color& hsv);
void RGBtoHEX(Color rgb, uint8_t* hex);

float GetTextWidth(const char* text, float height, Font* font);
void DrawText(const char* text, float height, float x, float y, Color rgb, float alpha, float angle, Font* font);
void DrawTextCentered(const char* text, float height, float x, float y, Color rgb, float alpha, Font* font);
void DrawTextRight(const char* text, float height, float x, float y, Color rgb, float alpha, Font* font);
void DrawRectangle(float width, float height, float x, float y, Color rgb, float alpha, float angle, Material* material);
void DrawGradientHorizontal(float width, float height, float x, float y, Color leftRgb, float leftAlpha, Color rightRgb, float rightAlpha);
void DrawGradientVertical(float width, float height, float x, float y, Color topRgb, float topAlpha, Color bottomRgb, float bottomAlpha);

unsigned int HashString(const char* string, unsigned int hash);

namespace r_rendercmds
{
    void start();
}