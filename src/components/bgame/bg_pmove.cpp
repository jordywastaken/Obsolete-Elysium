
#include "bg_weapons.h"
#include "globals.h"
#include "game/game.h"
#include "utils/hooking.h"

bool noclip = false, ufo = false;

void PmoveSingle_Hook(CallContext_t& ctx)
{
    // .text:0x1337 F8 01 01 F0 std       r0, 0x1E0+arg_10(r1) # Store Double Word
    uint64_t* stackFrame = ctx.GetGpr<uint64_t*>(1);
    stackFrame[0x1F0 / sizeof(uint64_t)] = ctx.r0;

    auto pm = ctx.GetGpr<pmove_t*>(3);

    if (pm->ps->clientNum == cg->clientNum && (noclip || ufo))
    {
        float forwardMove = static_cast<float>(pm->cmd.forwardmove) / 128.0;
        float rightMove = static_cast<float>(pm->cmd.rightmove) / 128.0;
        float upMove = 0.0;

        if (pm->cmd.buttonBits[0] & UCMD_MASK_GRENADE_LETHAL)
            upMove += 1.0;
        if (pm->cmd.buttonBits[0] & UCMD_MASK_GRENADE_TACTICAL)
            upMove -= 1.0;

        float speed = pm->cmd.buttonBits[0] & UCMD_MASK_SPRINT ? 20.0 : 10.0;
        forwardMove *= speed;
        rightMove *= speed;
        upMove *= speed;

        if (noclip)
        {
            vec3_t forward, right;
            AngleVectors(&pm->ps->viewangles, &forward, &right, 0);

            pm->ps->velocity.x = forward.x * forwardMove + right.x * rightMove;
            pm->ps->velocity.y = forward.y * forwardMove + right.y * rightMove;
            pm->ps->velocity.z = forward.z * forwardMove + right.z * rightMove;
            pm->ps->origin.x += pm->ps->velocity.x;
            pm->ps->origin.y += pm->ps->velocity.y;
            pm->ps->origin.z += pm->ps->velocity.z;
        }
        else if (ufo)
        {
            vec3_t forward, right, up;
            AngleVectors(&pm->ps->viewangles, &forward, &right, &up);

            pm->ps->velocity.x = forward.x * forwardMove + right.x * rightMove;
            pm->ps->velocity.y = forward.y * forwardMove + right.y * rightMove;
            pm->ps->velocity.z = up.z * upMove;
            pm->ps->origin.x += pm->ps->velocity.x;
            pm->ps->origin.y += pm->ps->velocity.y;
            pm->ps->origin.z += pm->ps->velocity.z;
        }
    }
}

void PM_AirMove_Hook(CallContext_t& ctx)
{
    // .text:0x1337 F8 01 01 30 std       r0, 0x120+arg_10(r1) # Store Double Word
    uint64_t* stackFrame = ctx.GetGpr<uint64_t*>(1);
    stackFrame[0x130 / sizeof(uint64_t)] = ctx.r0;

    auto pm = ctx.GetGpr<pmove_t*>(3);

    if (pm->ps->clientNum == cg->clientNum && (noclip || ufo))
    {
        ctx.r1 += 0x120;
        ctx.lr = ctx.r0;
    }
}

void PM_WalkMove_Hook(CallContext_t& ctx)
{
    // .text:0x1337 F8 01 01 50 std       r0, 0x140+arg_10(r1) # Store Double Word
    uint64_t* stackFrame = ctx.GetGpr<uint64_t*>(1);
    stackFrame[0x150 / sizeof(uint64_t)] = ctx.r0;

    auto pm = ctx.GetGpr<pmove_t*>(3);

    if (pm->ps->clientNum == cg->clientNum && (noclip || ufo))
    {
        ctx.r1 += 0x140;
        ctx.lr = ctx.r0;
    }
}

namespace bg_pmove
{
    void start()
    {
        hook::inject(0x1337, PmoveSingle_Hook);
        hook::inject(0x1337, PM_AirMove_Hook);
        hook::inject(0x1337, PM_WalkMove_Hook);
    }
}