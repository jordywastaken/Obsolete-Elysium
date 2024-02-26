
#pragma once

#include "game/game.h"

union PackedUnitVec
{
    unsigned int packed;
};

struct GfxVertex
{
    vec4_t xyzw;
    GfxColor color;
    vec2_t texCoord;
    PackedUnitVec normal;
};

struct materialCommands_t
{
    GfxVertex verts[2048];
    unsigned short indices[3072];
    int vertDeclType; 
    unsigned int vertexSize; 
    int indexCount; 
    int vertexCount; 
    int firstVertex; 
    int lastVertex; 
    bool finishedFilling; 
    int primType;
};

extern materialCommands_t& tess;



struct PathData
{
    GfxVertex* front();
    int size();
    bool push(GfxVertex& vertex);
    bool push(float x, float y, float s, float t, unsigned int color, bool applyScaling);
    void clear();


    static constexpr int maxVertex = 150;

    GfxVertex verts[maxVertex];
    int current;
};

float R_InvertLength(vec2_t v, float fail_value);
void R_SetVertex2d(GfxVertex* vert, float x, float y, float s, float t, unsigned int color, bool applyScaling);
void RB_SetTessTechnique(Material* material, int techType, int primType);
void RB_TessOverflow();
void RB_TessAddQuadIndices(int vertexCount);
void RB_BeginPolygon(Material* material, int vertexCount, int indexCount);
void RB_DrawPolygonFilled(int pointCount, GfxVertex* verts, Material* material);
void RB_DrawPolygonOutline(int pointCount, GfxVertex* verts, Material* material, float thickness, unsigned int color);
void RB_DrawPolygonInnerGradient(int pointCount, GfxVertex* verts, Material* material, vec2_t center);
void RB_DrawPolygonOuterGradient(int pointCount, GfxVertex* verts, Material* material, vec2_t center);

namespace rb_backend
{
    void start();
}