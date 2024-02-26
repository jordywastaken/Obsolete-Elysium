
#include "r_rendercmds.h"
#include "globals.h"
#include "components/database/db_registry.h"
#include "utils/hooking.h"
#include "utils/imports.h"

template<typename T>
static inline void swap(T& a, T& b)
{
    T tmp = a; a = b; b = tmp;
}

void HSVtoRGB(Color hsv, Color& rgb)
{
    if (hsv.s == 0.0)
    {
        // gray
        rgb = hsv.v;
        return;
    }

    hsv.h = fmodf(hsv.h / 360.0, 1.0) / (60.0 / 360.0);
    int   i = static_cast<int>(hsv.h);
    float f = hsv.h - static_cast<float>(i);
    float p = hsv.v * (1.0 - hsv.s);
    float q = hsv.v * (1.0 - hsv.s * f);
    float t = hsv.v * (1.0 - hsv.s * (1.0 - f));

    switch (i)
    {
    case(0): rgb = { hsv.v, t, p }; break;
    case(1): rgb = { q, hsv.v, p }; break;
    case(2): rgb = { p, hsv.v, t }; break;
    case(3): rgb = { p, q, hsv.v }; break;
    case(4): rgb = { t, p, hsv.v }; break;
    case(5): default: rgb = { hsv.v, p, q }; break;
    }
}

void RGBtoHSV(Color rgb, Color& hsv)
{
    float K = 0.f;
    if (rgb.g < rgb.b)
    {
        swap(rgb.g, rgb.b);
        K = -1.f;
    }
    if (rgb.r < rgb.g)
    {
        swap(rgb.r, rgb.g);
        K = -2.f / 6.f - K;
    }

    float chroma = rgb.r - (rgb.g < rgb.b ? rgb.g : rgb.b);
    hsv.h = __builtin_fabsf(K + (rgb.g - rgb.b) / (6.0 * chroma + 1e-20)) * 360.0;
    hsv.s = chroma / (rgb.r + 1e-20);
    hsv.v = rgb.r;
}

void RGBtoHEX(Color rgb, uint8_t* hex)
{
    hex[0] = static_cast<uint8_t>(rgb.r * 255.0);
    hex[1] = static_cast<uint8_t>(rgb.g * 255.0);
    hex[2] = static_cast<uint8_t>(rgb.b * 255.0);
}

void ScaleToViewport(float* x, float* y)
{
    ScreenPlacement* scrPlace = ScrPlace_GetView(0);

    if (x) *x *= scrPlace->realViewportSize.x / 1280.0;
    if (y) *y *= scrPlace->realViewportSize.y / 720.0;
}

float GetTextWidth(const char* text, float height, Font* font)
{
    if (!font)
        return 0.0;

    float xScale = height / static_cast<float>(font->pixelHeight);
    return static_cast<float>(R_TextWidth(0, text, 0x7fffffff, font)) * xScale;
}

void DrawText(const char* text, float height, float x, float y, Color rgb, float alpha, float angle, Font* font)
{
    if (!font)
        return;

    vec4_t color
    {
        rgb.r,
        rgb.g,
        rgb.b,
        alpha
    };

    y += height;

    float xScale = height / static_cast<float>(font->pixelHeight);
    float yScale = xScale;

    ScaleToViewport(&x, &y); 
    ScaleToViewport(&xScale, & yScale);
    R_AddCmdDrawTextInternal(text, 0x7fffffff, font, x, y, xScale, yScale, 0.0, &color, 0);
}

void DrawTextCentered(const char* text, float height, float x, float y, Color rgb, float alpha, Font* font)
{
    float width = GetTextWidth(text, height, font);
    DrawText(text, height, x - width * 0.5, y, rgb, alpha, 0.0, font);
}

void DrawTextRight(const char* text, float height, float x, float y, Color rgb, float alpha, Font* font)
{
    float width = GetTextWidth(text, height, font);
    DrawText(text, height, x - width, y, rgb, alpha, 0.0, font);
}

void DrawRectangle(float width, float height, float x, float y, Color rgb, float alpha, float angle, Material* material)
{
    if (!material)
        return;

    vec4_t color
    {
        rgb.r,
        rgb.g,
        rgb.b,
        alpha
    };

    ScaleToViewport(&x, &y);
    ScaleToViewport(&width, &height);
    CG_DrawRotatedPicPhysical(ScrPlace_GetView(0), x, y, width, height, angle, &color, material);
}

void DrawGradientHorizontal(float width, float height, float x, float y, Color leftRgb, float leftAlpha, Color rightRgb, float rightAlpha)
{
    DrawRectangle(width, height, x, y, leftRgb, leftAlpha, 180.0, gradient);
    DrawRectangle(width, height, x, y, rightRgb, rightAlpha, 0.0, gradient);
}

void DrawGradientVertical(float width, float height, float x, float y, Color topRgb, float topAlpha, Color bottomRgb, float bottomAlpha)
{
    DrawRectangle(height, width, x - (height - width) * 0.5, y + (height - width) * 0.5, topRgb, topAlpha, 270.0, gradient);
    DrawRectangle(height, width, x - (height - width) * 0.5, y + (height - width) * 0.5, bottomRgb, bottomAlpha, 90.0, gradient);
}

unsigned int HashString(const char* string, unsigned int hash)
{
    unsigned int result = hash;

    while (*string)
    {
        result = (33 * result) ^ (*string | 0x20);
        string++;
    }
    return result;
}

unsigned int hashTable[2048];

void R_AddCmdDrawUIQuadsInternal_Hook(CallContext_t& ctx)
{
    // .text:0x1337 F8 01 01 40 std       r0, 0x130+arg_10(r1) # Store Double Word
    ctx.GetGpr<uint64_t*>(1)[0x140 / sizeof(uint64_t)] = ctx.r0;

    auto material = ctx.GetGpr<Material*>(4);
    auto color    = ctx.GetGpr<GfxColor*>(8);

    auto name = material ? material->info.name : 0;

    if (name && color)
    {
        auto hash = HashString(name, 0);

        for (auto& entry : hashTable)
        {
            if (entry == hash)
                break;

            if (!entry)
            {
                printf("0x%08X, // %s\n", hash, name);
                entry = hash;
                break;
            }
        }

        /*GfxColor colorOverride;
        colorOverride.r = (unsigned char)(g_colorPrimary.r * 255.0);
        colorOverride.g = (unsigned char)(g_colorPrimary.g * 255.0);
        colorOverride.b = (unsigned char)(g_colorPrimary.b * 255.0);
        colorOverride.a = color->a;

        if (!I_stricmp(name, "ui_normal") || 
            !I_stricmp(name, "ui_add") || 
            !I_stricmp(name, "ui_multiply") ||
            !I_stricmp(name, "uie_feather_edges") ||
            !I_stricmp(name, "uie_feather_add") ||
            !I_stricmp(name, "uie_feather_blend") ||
            !I_stricmp(name, "uie_nineslice_add") ||
            !I_stricmp(name, "uie_nineslice_normal") ||
            !I_stricmp(name, "uie_scene_blur_pass_1_nineslice") ||
            !I_stricmp(name, "white") ||
            !I_stricmp(name, "cinematic"))
            ctx.GetGpr<GfxColor*>(8) = &colorOverride;*/
    }
}

namespace r_rendercmds
{
    void start()
    {
        hook::inject(0x1337, R_AddCmdDrawUIQuadsInternal_Hook);
    }
}