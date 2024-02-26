
#include "bg_weapons_ammo.h"
#include "globals.h"
#include "game/game.h"
#include "utils/hooking.h"

void BG_AddAmmoToClip_Hook(CallContext_t& ctx)
{
    // .text:0x1337 F8 01 00 A0 std       r0, 0x90+arg_10(r1) # Store Double Word
    uint64_t* stackFrame = ctx.GetGpr<uint64_t*>(1);
    stackFrame[0xA0 / sizeof(uint64_t)] = ctx.r0;

    auto ps     = ctx.GetGpr<const playerState_s*>(3);
    auto amount = ctx.GetGpr<int>(5);

    if (ps->clientNum == cg->clientNum && amount < 0)
    {
        ctx.r1 += 0x90;
        ctx.lr = ctx.r0;
    }
}

void BG_GetAmmoInClip_Hook(CallContext_t& ctx)
{
    // .text:0x1337 F8 01 00 90 std       r0, 0x80+arg_10(r1) # Store Double Word
    uint64_t* stackFrame = ctx.GetGpr<uint64_t*>(1);
    stackFrame[0x90 / sizeof(uint64_t)] = ctx.r0;

    auto ps = ctx.GetGpr<const playerState_s*>(3);

    if (ps->clientNum == cg->clientNum)
    {
        ctx.r3 = 999;
        ctx.r1 += 0x80;
        ctx.lr = ctx.r0;
    }
}

namespace bg_weapons_ammo
{
    void start()
    {
        hook::inject(0x1337, BG_AddAmmoToClip_Hook);
        //hook::inject(0x1337, BG_GetAmmoInClip_Hook);
    }
}