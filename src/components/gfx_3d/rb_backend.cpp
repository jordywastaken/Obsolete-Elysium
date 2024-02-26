
#include "rb_backend.h"
#include "r_rendercmds.h"
#include "components/ps3/ps3_gamepad.h"
#include "utils/hooking.h"
#include "utils/imports.h"

materialCommands_t& tess = *(materialCommands_t*)(0x1337);
PathData g_pathData;
vec2_t g_viewportScale;

GfxVertex* PathData::front()
{
    return &verts[0];
}

int PathData::size()
{
    return current;
}

bool PathData::push(GfxVertex& vertex)
{
    if (current >= maxVertex)
        return false;


    verts[current++] = vertex;
    return true;
}

bool PathData::push(float x, float y, float s, float t, unsigned int color, bool applyScaling)
{
    if (current >= maxVertex)
        return false;

    R_SetVertex2d(&verts[current++], x, y, s, t, color, applyScaling);
    return true;
}

void PathData::clear()
{
    _sys_memset(verts, 0, sizeof(GfxVertex) * maxVertex);
    current = 0;
}


float R_InvertLength(vec2_t v, float fail_value)
{
    float d = (v.x * v.x) + (v.y * v.y);

    if (d > 0.0)
        return 1.0 / __builtin_fsqrts(d);

    return fail_value;
}

vec2_t R_RotatePoint(vec2_t center, vec2_t point, float angle)
{
    float theta = angle * 0.01745329251;
    float c = cos(theta);
    float s = sin(theta);

    return
    {
        center.x + (point.x - center.x) * c - (point.y - center.y) * s,
        center.y + (point.x - center.x) * s + (point.y - center.y) * c,
    };
}

void R_SetVertex2d(GfxVertex* vert, float x, float y, float s, float t, unsigned int color, bool applyScaling)
{
    vert->xyzw.x = applyScaling ? x * g_viewportScale.x : x;
    vert->xyzw.y = applyScaling ? y * g_viewportScale.y : y;
    vert->xyzw.z = 0.0;
    vert->xyzw.w = 1.0;
    vert->normal.packed = 0x7FC00000;
    vert->color.packed = color;
    vert->texCoord.x = s;
    vert->texCoord.y = t;
}

void RB_SetTessTechnique(Material* material, int techType, int primType)
{
    hook::invoke<void>(0x3DC700, material, techType, primType);
}

void RB_TessOverflow()
{
    hook::invoke<void>(0x3DC794);
}


void RB_TessAddQuadIndices(int vertexCount)
{
    tess.indices[tess.indexCount++] = vertexCount + 3;
    tess.indices[tess.indexCount++] = vertexCount;
    tess.indices[tess.indexCount++] = vertexCount + 2;
    tess.indices[tess.indexCount++] = vertexCount + 2;
    tess.indices[tess.indexCount++] = vertexCount;
    tess.indices[tess.indexCount++] = vertexCount + 1;
}

void RB_BeginPolygon(Material* material, int vertexCount, int indexCount)
{
    RB_SetTessTechnique(material, 2, 5);

    if (tess.vertexCount + vertexCount > 2048 || tess.indexCount + indexCount > 3072)
    {
        RB_TessOverflow();
    }
}

void RB_DrawPolygonFilled(int pointCount, GfxVertex* verts, Material* material)
{
    const int vertexCount = pointCount;
    const int indexCount = (pointCount - 2) * 3;

    RB_BeginPolygon(material, vertexCount, indexCount);

    for (int i = 0; i < pointCount - 2; i++)
    {
        tess.indices[tess.indexCount] = tess.vertexCount;
        tess.indices[tess.indexCount + 1] = tess.vertexCount + i + 1;
        tess.indices[tess.indexCount + 2] = tess.vertexCount + i + 2;
        tess.indexCount += 3;
    }

    for (int i = 0; i < pointCount; i++)
    {
        tess.verts[tess.vertexCount++] = verts[i];
    }
}

void RB_DrawPolygonOutline(int pointCount, GfxVertex* verts, Material* material, float thickness, unsigned int color)
{
    for (int i = 0; i < pointCount; i++)
    {
        RB_BeginPolygon(material, 4, 6);

        int j = (i + 1 == pointCount) ? 0 : i + 1;

        vec2_t p1{ verts[i].xyzw.x,  verts[i].xyzw.y };
        vec2_t p2{ verts[j].xyzw.x,  verts[j].xyzw.y };

        vec2_t diff
        {
            p2.x - p1.x,
            p2.y - p1.y
        };

        float invert = R_InvertLength(diff, 1.0);
        diff.x *= invert;
        diff.y *= invert;

        const float dx = diff.x * (thickness * 0.5);
        const float dy = diff.y * (thickness * 0.5);

        RB_TessAddQuadIndices(tess.vertexCount);

        R_SetVertex2d(&tess.verts[tess.vertexCount++], p1.x + dy, p1.y - dx, 0.0, 1.0, color, false);
        R_SetVertex2d(&tess.verts[tess.vertexCount++], p2.x + dy, p2.y - dx, 0.0, 1.0, color, false);
        R_SetVertex2d(&tess.verts[tess.vertexCount++], p2.x - dy, p2.y + dx, 0.0, 1.0, color, false);
        R_SetVertex2d(&tess.verts[tess.vertexCount++], p1.x - dy, p1.y + dx, 0.0, 1.0, color, false);
    }
}

void RB_DrawPolygonInnerGradient(int pointCount, GfxVertex* verts, Material* material, vec2_t center)
{
    for (int i = 0; i < pointCount; i++)
    {
        RB_BeginPolygon(material, 4, 6);

        int j = (i + 1 == pointCount) ? 0 : i + 1;

        vec2_t p1{ verts[i].xyzw.x,  verts[i].xyzw.y };
        vec2_t p2{ verts[j].xyzw.x,  verts[j].xyzw.y };

        RB_TessAddQuadIndices(tess.vertexCount);

        const unsigned int alpha_mask = 0xFF;

        R_SetVertex2d(&tess.verts[tess.vertexCount++], p1.x, p1.y, 0.0, 1.0, verts[i].color.packed & ~alpha_mask, false);
        R_SetVertex2d(&tess.verts[tess.vertexCount++], p2.x, p2.y, 0.0, 1.0, verts[j].color.packed & ~alpha_mask, false);
        R_SetVertex2d(&tess.verts[tess.vertexCount++], center.x, center.y, 0.0, 1.0, verts[j].color.packed, false);
        R_SetVertex2d(&tess.verts[tess.vertexCount++], center.x, center.y, 0.0, 1.0, verts[i].color.packed, false);
    }
}

void RB_DrawPolygonOuterGradient(int pointCount, GfxVertex* verts, Material* material, vec2_t center)
{
    for (int i = 0; i < pointCount; i++)
    {
        RB_BeginPolygon(material, 4, 6);

        int j = (i + 1 == pointCount) ? 0 : i + 1;

        vec2_t p1{ verts[i].xyzw.x,  verts[i].xyzw.y };
        vec2_t p2{ verts[j].xyzw.x,  verts[j].xyzw.y };

        RB_TessAddQuadIndices(tess.vertexCount);

        const unsigned int alpha_mask = 0xFF;

        R_SetVertex2d(&tess.verts[tess.vertexCount++], p1.x, p1.y, 0.0, 1.0, verts[i].color.packed, false);
        R_SetVertex2d(&tess.verts[tess.vertexCount++], p2.x, p2.y, 0.0, 1.0, verts[j].color.packed, false);
        R_SetVertex2d(&tess.verts[tess.vertexCount++], center.x, center.y, 0.0, 1.0, verts[j].color.packed & ~alpha_mask, false);
        R_SetVertex2d(&tess.verts[tess.vertexCount++], center.x, center.y, 0.0, 1.0, verts[i].color.packed & ~alpha_mask, false);
    }
}


void RB_DrawStretchPic(float x, float y, float width, float height, float s0, float t0, float s1, float t1, unsigned int color, int type, Material* material)
{
    g_pathData.push(x, y, s0, t0, color, true);
    g_pathData.push(x + width, y, s1, t0, color, true);
    g_pathData.push(x + width, y + height, s1, t1, color, true);
    g_pathData.push(x, y + height, s0, t1, color, true);

    vec2_t center
    {
        x + width * 0.5f,
        y + height * 0.5f
    };

    switch (type)
    {
    case(0): RB_DrawPolygonFilled(g_pathData.size(), g_pathData.front(), material); break;
    case(1): RB_DrawPolygonInnerGradient(g_pathData.size(), g_pathData.front(), material, center); break;
    case(2): RB_DrawPolygonOuterGradient(g_pathData.size(), g_pathData.front(), material, center); break;
    }
    g_pathData.clear();
}

void RB_DrawRoundedRect(float x, float y, float width, float height, float rounding, bool roundTopLeft, bool roundTopRight, bool roundBottomRight, bool roundBottomLeft, unsigned int color, int type, Material* material)
{
    const int roundingVertexCount = 35;

    auto DoArc = [&](vec2_t center, vec2_t start)
    {
        if (center.x == start.x && center.y == start.y)
            return;

        float angle = 90.0 / static_cast<float>(roundingVertexCount);
        float theta = angle * 0.01745329251;

        for (int i = 0; i < roundingVertexCount; i++)
        {
            float r = static_cast<float>(i) * theta;
            float s = sin(r);
            float c = cos(r);

            vec2_t point
            {
                center.x + (start.x - center.x) * c - (start.y - center.y) * s,
                center.y + (start.x - center.x) * s + (start.y - center.y) * c,
            };

            g_pathData.push(point.x, point.y, 0.0, 1.0, color, true);
        }
    };

    if (rounding == 0.0)
    {
        RB_DrawStretchPic(x, y, width, height, 0.0, 0.0, 1.0, 1.0, color, type, material);
    }
    else
    {
        float rtl = roundTopLeft ? rounding : 0.0;
        float rtr = roundTopRight ? rounding : 0.0;
        float rbr = roundBottomRight ? rounding : 0.0;
        float rbl = roundBottomLeft ? rounding : 0.0;

        DoArc({ x + rtl, y + rtl }, { x, y + rtl }); // Top left
        g_pathData.push(x + rtl, y, 0.0, 1.0, color, true); // Top

        DoArc({ x + width - rtr, y + rtr }, { x + width - rtr, y }); // Top right
        g_pathData.push(x + width, y + rtr, 0.0, 1.0, color, true); // Right

        DoArc({ x + width - rbr, y + height - rbr }, { x + width, y + height - rbr }); // Bottom right
        g_pathData.push(x + width - rbr, y + height, 0.0, 1.0, color, true); // Bottom

        DoArc({ x + rbl, y + height - rbl }, { x + rbl, y + height }); // Bottom left
        g_pathData.push(x, y + height - rbl, 0.0, 1.0, color, true); // Left

        vec2_t center
        {
            (x + width * 0.5f) * g_viewportScale.x,
            (y + height * 0.5f) * g_viewportScale.y
        };

        switch (type)
        {
        case(0): RB_DrawPolygonFilled(g_pathData.size(), g_pathData.front(), material); break;
        case(1): RB_DrawPolygonInnerGradient(g_pathData.size(), g_pathData.front(), material, center); break;
        case(2): RB_DrawPolygonOuterGradient(g_pathData.size(), g_pathData.front(), material, center); break;
        }
        g_pathData.clear();
    }
}

void RB_DrawRoundedRectOutline(float x, float y, float width, float height, float thickness, float rounding, bool roundTopLeft, bool roundTopRight, bool roundBottomRight, bool roundBottomLeft, unsigned int color, Material* material)
{
    const int roundingVertexCount = 35;

    auto DoArc = [&](vec2_t center, vec2_t start)
    {
        if (center.x == start.x && center.y == start.y)
            return;

        float angle = 90.0 / static_cast<float>(roundingVertexCount);
        float theta = angle * 0.01745329251;

        for (int i = 0; i < roundingVertexCount; i++)
        {
            float r = static_cast<float>(i) * theta;
            float s = sin(r);
            float c = cos(r);

            vec2_t point
            {
                center.x + (start.x - center.x) * c - (start.y - center.y) * s,
                center.y + (start.x - center.x) * s + (start.y - center.y) * c,
            };

            g_pathData.push(point.x, point.y, 0.0, 1.0, color, true);
        }
    };

    if (rounding == 0.0)
    {

    }
    else
    {
        float rtl = roundTopLeft ? rounding : 0.0;
        float rtr = roundTopRight ? rounding : 0.0;
        float rbr = roundBottomRight ? rounding : 0.0;
        float rbl = roundBottomLeft ? rounding : 0.0;

        DoArc({ x + rtl, y + rtl }, { x, y + rtl }); // Top left
        g_pathData.push(x + rtl, y, 0.0, 1.0, color, true); // Top

        DoArc({ x + width - rtr, y + rtr }, { x + width - rtr, y }); // Top right
        g_pathData.push(x + width, y + rtr, 0.0, 1.0, color, true); // Right

        DoArc({ x + width - rbr, y + height - rbr }, { x + width, y + height - rbr }); // Bottom right
        g_pathData.push(x + width - rbr, y + height, 0.0, 1.0, color, true); // Bottom

        DoArc({ x + rbl, y + height - rbl }, { x + rbl, y + height }); // Bottom left
        g_pathData.push(x, y + height - rbl, 0.0, 1.0, color, true); // Left

        RB_DrawPolygonOutline(g_pathData.size(), g_pathData.front(), material, thickness, color);
        g_pathData.clear();
    }
}

void RB_DrawCircle(float x, float y, float radius, unsigned int color, int type, Material* material)
{
    for (int i = 0; i < PathData::maxVertex; i++)
    {
        float angle = static_cast<float>(i) / static_cast<float>(PathData::maxVertex) * 360.0;
        float theta = angle * 0.01745329251;

        vec2_t point
        {
            x + sin(theta) * radius,
            y + cos(theta) * radius
        };

        GfxVertex vertex;
        R_SetVertex2d(&vertex, point.x, point.y, 0.0, 1.0, color, true);
        g_pathData.push(vertex);
    }

    switch (type)
    {
    case(0): RB_DrawPolygonFilled(g_pathData.size(), g_pathData.front(), material); break;
    case(1): RB_DrawPolygonInnerGradient(g_pathData.size(), g_pathData.front(), material, { x * g_viewportScale.x, y * g_viewportScale.y }); break;
    case(2): RB_DrawPolygonOuterGradient(g_pathData.size(), g_pathData.front(), material, { x * g_viewportScale.x, y * g_viewportScale.y }); break;
    }
    g_pathData.clear();
}

void RB_DrawCircleOutline(float x, float y, float radius, float thickness, unsigned int color, Material* material)
{
    for (int i = 0; i < PathData::maxVertex; i++)
    {
        float angle = static_cast<float>(i) / static_cast<float>(PathData::maxVertex) * 360.0;
        float theta = angle * 0.01745329251;

        vec2_t point
        {
            x + sin(theta) * radius,
            y + cos(theta) * radius
        };

        GfxVertex vertex;
        R_SetVertex2d(&vertex, point.x, point.y, 0.0, 1.0, color, true);
        g_pathData.push(vertex);
    }

    RB_DrawPolygonOutline(g_pathData.size(), g_pathData.front(), material, thickness, color);
    g_pathData.clear();
}

void DrawText2D(const char* text, float x, float y, float w, Font* font, float xScale, float yScale, float sinAngle, float cosAngle, const GfxColor color, int maxLength, int renderFlags, int cursorPos, unsigned char cursorLetter, float padding, GfxColor glowForcedColor, int fxBirthTime, int fxLetterTime, int fxDecayStartTime, int fxDecayDuration, int fxRedactDecayStartTime, int fxRedactDecayDuration, const Material* fxMaterial, const Material* fxMaterialGlow)
{
    hook::invoke<void>(0x1337, text, x, y, w, font, xScale, yScale, sinAngle, cosAngle, color, maxLength, renderFlags, cursorPos, cursorLetter, padding, glowForcedColor, fxBirthTime, fxLetterTime, fxDecayStartTime, fxDecayDuration, fxRedactDecayStartTime, fxRedactDecayDuration, fxMaterial, fxMaterialGlow);
}

void RB_DrawText(const char* text, float height, float x, float y, unsigned int color, Font* font)
{
    float xScale = (height / static_cast<float>(font->pixelHeight)) / 1.1;
    float yScale = xScale;

    y += height;

    x *= g_viewportScale.x;
    y *= g_viewportScale.y;
    xScale *= g_viewportScale.x;
    yScale *= g_viewportScale.y;

    DrawText2D(text, x, y, 1.0,
        font,
        xScale,
        yScale,
        0.0,
        1.0,
        { color },
        0x7FFFFFFF,
        0,
        -1,
        0,
        0.0,
        {},
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0);
}

void RB_ExecuteRenderCommandsLoop_Hook(CallContext_t& ctx)
{
    // .text:0x1337 E8 01 00 B0 ld        r0, 0xA0+arg_10(r1) # Load Double Word
    ctx.r0 = ctx.GetGpr<uint64_t*>(1)[0xB0 / sizeof(uint64_t)];

    // Update our viewport
    ScreenPlacement* scrPlace = ScrPlace_GetView(0);
    g_viewportScale.x = scrPlace->realViewportSize.x / 1280.0;
    g_viewportScale.y = scrPlace->realViewportSize.y / 720.0;

    g_pathData.clear();


    Material* white;
    LUI_Interface_RegisterMaterial("white", &white);


    static float x = 640.0;
    static float y = 360.0;

    x += GetAnalogAxis(ANALOG_RIGHT_X) * 5.0;
    y += GetAnalogAxis(ANALOG_RIGHT_Y) * 5.0;


    vec2_t size{ 250.0, 160.0 };
    vec2_t base{ x - size.x * 0.5f, y - size.y * 0.5f };

    RB_DrawRoundedRect(base.x, base.y, size.x, size.y, 15.0, true, false, true, false, 0x101010DD, 0, white);
    RB_DrawRoundedRectOutline(base.x, base.y, size.x, size.y, 2.0, 15.0, true, false, true, false, 0xFFFFFF80, white);

    RB_DrawCircle(x, y, 60.0, 0xFF0000FF, 1, white);
}


namespace rb_backend
{
    void start()
    {
        //hook::inject(0x1337, RB_ExecuteRenderCommandsLoop_Hook);
    }
}