
#include "cg_weapons.h"
#include "globals.h"
#include "utils/hooking.h"
#include "utils/imports.h"
#include <cell/gcm.h>

cell::Gcm::Inline::CellGcmContext& g_gcmContext = *reinterpret_cast<cell::Gcm::Inline::CellGcmContext*>(0x1337);

void CG_SetupWeaponCamoRender_Hook(CallContext_t& ctx)
{
    // .text:0x1337 61 57 00 00 mr        r23, r10      # Move Register
    ctx.r23 = ctx.r10;

    if (Config->SpoofCamo)
    {
        ctx.r7 = Config->SpoofedCamo;
    }
}

void RB_DrawDepthPrepassCommandBuffer_Hook(CallContext_t& ctx)
{
    // .text:0x1337 60 00 00 00 nop                     # No Operation

    auto label = ctx.GetGpr<const char*>(31);

    if (Config->ViewModelWireframe && _sys_strcmp(label, "DEPTH HACK PREPASS") == 0)
        ctx.r3 = false;
}

void R_DrawXModelRigidModelSurf_Hook(CallContext_t& ctx)
{
    // .text:0x1337 91 64 00 00 stw       r11, 0(r4)    # Store Word

    auto r4 = ctx.GetGpr<uint32_t*>(4);

    if (Config->ViewModelWireframe)
    {
        *r4 = 4;
        g_gcmContext.SetLineWidth(Config->ViewModelWireSize);
    }
    else
    {
        *r4 = ctx.GetGpr<uint32_t>(11);
    }
}


void BG_WeaponFireRecoil_Hook(CallContext_t& ctx)
{
    // .text:0x1337 F8 01 01 50 std       r0, 0x140+arg_10(r1) # Store Double Word
    uint64_t* stackFrame = ctx.GetGpr<uint64_t*>(1);
    stackFrame[0x150 / sizeof(uint64_t)] = ctx.r0;

    if (Config->NoRecoil)
    {
        ctx.r1 += 0x140;
        ctx.lr = ctx.r0;
    }
}

void BG_SetWeaponMovementAngles_Hook(CallContext_t& ctx)
{
    // .text:0x1337 F8 01 00 C0 std       r0, 0xB0+arg_10(r1) # Store Double Word
    uint64_t* stackFrame = ctx.GetGpr<uint64_t*>(1);
    stackFrame[0xC0 / sizeof(uint64_t)] = ctx.r0;

    if (Config->NoSway)
    {
        ctx.r1 += 0xB0;
        ctx.lr = ctx.r0;
    }
}

void BG_CalculateViewMovementAngles_Hook(CallContext_t& ctx)
{

}

void CG_KickAngles_Hook(CallContext_t& ctx)
{
    // .text:0x1337 F8 01 01 30 std       r0, 0x120+arg_10(r1) # Store Double Word
    uint64_t* stackFrame = ctx.GetGpr<uint64_t*>(1);
    stackFrame[0x130 / sizeof(uint64_t)] = ctx.r0;

    if (Config->NoKick)
    {
        ctx.r1 += 0x120;
        ctx.lr = ctx.r0;
    }
}

namespace cg_weapons
{
    void start()
    {
        hook::inject(0x1337, CG_SetupWeaponCamoRender_Hook);
        hook::inject(0x1337, RB_DrawDepthPrepassCommandBuffer_Hook);
        hook::inject(0x1337, R_DrawXModelRigidModelSurf_Hook);

        hook::inject(0x1337, BG_WeaponFireRecoil_Hook);
        hook::inject(0x1337, BG_SetWeaponMovementAngles_Hook);
        //hook::inject(0x1337, BG_CalculateViewMovementAngles_Hook);
        hook::inject(0x1337, CG_KickAngles_Hook);
    }
}