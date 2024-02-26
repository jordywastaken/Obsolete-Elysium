
#include "aim_target.h"
#include "globals.h"
#include "components/scheduler.h"
#include "components/database/db_registry.h"
#include "utils/hooking.h"
#include <float.h>

const char* j_bones[TARGET_BONE_COUNT]
{
    "j_helmet",
    "j_head",
    "j_neck",
    "j_spineupper",
    "j_spinelower",
    "j_mainroot",
    "j_shoulder_le",
    "j_shoulder_ri",
    "j_elbow_le",
    "j_elbow_ri",
    "j_wrist_le",
    "j_wrist_ri",
    "j_knee_le",
    "j_knee_ri",
    "j_ankle_le",
    "j_ankle_ri"
};

int currentTarget = -1;
TargetInfo targets[12];
vec3_t viewOrigin;
vec3_t aimAngles;

int GetBoneIndex(DObj* obj, const char* bone)
{
    unsigned int name = SL_FindString(bone);
    XModel * model = obj->models[0];

    for (int i = 0; i < model->numBones; i++)
    {
        if (model->boneNames[i] == name)
            return i;
    }
    return 0;
}

int GetTraceHitId(trace_t* trace)
{
    switch (trace->hitType)
    {
    case(TRACE_HITTYPE_ENTITY):
        return trace->hitId;
    case(TRACE_HITTYPE_DYNENT_MODEL):
    case(TRACE_HITTYPE_DYNENT_BRUSH):
    case(TRACE_HITTYPE_GLASS):
        return 702;
    default:
        return 703;
    }
}

bool IsEntityAlive(centity_t* ent) // From CScr_IsAlive
{
    int clientNum = (ent->nextState.clientNum >> 8) & 0xF;

    switch (ent->nextState.eType)
    {
    case(ET_PLAYER):
        return (ent->nextState.lerp.eFlags & 0x40000) == 0 && cg->clientInfo[clientNum].health > 0;
    case(ET_HELICOPTER):
    case(ET_PLANE):
    case(ET_VEHICLE):
    case(ET_VEHICLE_CORPSE):
    case(ET_ACTOR):
        return (ent->nextState.lerp.eFlags & 0x40000) == 0;
    }
    return false;
}

bool IsEntityEnemy(centity_t* ent)
{
    return !CG_IsEntityFriendlyNotEnemy(0, ent);
}

bool IsEntityVisible(centity_t* ent, vec3_t* bone)
{
    trace_t trace;
    CG_LocationalTrace(&trace, &viewOrigin, bone, cg->predictedPlayerState.clientNum, 0x803001, false, 0);
    return trace.fraction == 1.0 || GetTraceHitId(&trace) == ent->nextState.number;
}

void SimulateBulletFire(BulletFireParams* bp, BulletTraceResults* br)
{
    *reinterpret_cast<uint32_t*>(0x1337) = 0x4E800020; // CG_BulletHitEvent
    *reinterpret_cast<uint32_t*>(0x1337) = 0x4E800020; // Tracer_Spawn

    int weapon = cg->predictedPlayerState.weapon;
    CG_SimulateBulletFire_Internal(0, bp, br, weapon, cg->clientNum, &viewOrigin, false, false, BG_GetPenetrateType(weapon));

    *reinterpret_cast<uint32_t*>(0x1337) = 0x69420; // CG_BulletHitEvent
    *reinterpret_cast<uint32_t*>(0x1337) = 0x69420; // Tracer_Spawn

    /*int weapon = cg->predictedPlayerState.weapon;
    bool hasFMJ = CG_ClientHasPerk(0, cg->clientNum, 9);

    centity_t* self = &centity[cg->clientNum];
    WeaponDef* weaponDef = BG_GetWeaponDef(weapon);

    // If the bullet trace hit type is TRACE_HITTYPE_NONE, return
    if (!BulletTrace(0, bp, br, weapon, self->nextState.number, 0))
        return;

    // Make sure we can penetrate
    if (!BG_GetPenetrateType(weapon) || weaponDef->bBulletImpactExplode)
        return;

    if (br->trace.startsolid)
        return;

    // Loop through each walls
    for (int penetrationIndex = 0; penetrationIndex < sv_penetrationCount->current.integer; penetrationIndex++)
    {
        float maxDepth = BG_GetSurfacePenetrationDepth(BG_GetPenetrateType(weapon), br->surfaceType);

        if (hasFMJ)
            maxDepth *= perk_bulletPenetrationMultiplier->current.value;

        if (maxDepth <= 0.0)
            break;

        vec3_t lastHitPos = br->hitPos;

        if (!BG_AdvanceTrace(bp, br, 0.25999999))
            break;

        bool traceHit = BulletTrace(0, bp, br, weapon, self->nextState.number, br->surfaceType);

        BulletFireParams revBp = *bp;
        revBp.dir.x = -revBp.dir.x;
        revBp.dir.y = -revBp.dir.y;
        revBp.dir.z = -revBp.dir.z;
        revBp.start.x = bp->end.x;
        revBp.start.y = bp->end.y;
        revBp.start.z = bp->end.z;
        revBp.end.x = (0.0099999998 * revBp.dir.x) + lastHitPos.x;
        revBp.end.y = (0.0099999998 * revBp.dir.y) + lastHitPos.y;
        revBp.end.z = (0.0099999998 * revBp.dir.z) + lastHitPos.z;

        BulletTraceResults revBr = *br;
        revBr.trace.normal.x = -revBr.trace.normal.x;
        revBr.trace.normal.y = -revBr.trace.normal.y;
        revBr.trace.normal.z = -revBr.trace.normal.z;

        if (traceHit)
            BG_AdvanceTrace(&revBp, &revBr, 0.0099999998);

        bool revTraceHit = BulletTrace(0, &revBp, &revBr, weapon, self->nextState.number, revBr.surfaceType);
        bool allSolid = (revTraceHit && revBr.trace.allsolid) || (br->trace.startsolid && revBr.trace.startsolid);

        if (revTraceHit || allSolid)
        {
            float depth = allSolid ? revBp.start.distance(revBp.end) : revBr.hitPos.distance(lastHitPos);

            if (depth < 1.0)
                depth = 1.0;

            if (revTraceHit)
            {
                float penetrationDepth = BG_GetSurfacePenetrationDepth(BG_GetPenetrateType(weapon), revBr.surfaceType);

                if (hasFMJ)
                    penetrationDepth *= perk_bulletPenetrationMultiplier->current.value;

                maxDepth = (penetrationDepth - maxDepth) < 0.0 ? penetrationDepth : maxDepth;

                if (maxDepth <= 0.0)
                    break;
            }

            if (!hasFMJ)
                bp->damageMultiplier -= (depth / maxDepth);

            if (bp->damageMultiplier <= 0.0)
                break;
        }

        if (!traceHit)
            break;
    }*/
}

bool IsEntityHittable(centity_t* ent, vec3_t* bone)
{
    if (BG_GetPenetrateType(cg->predictedPlayerState.weapon) == PENETRATE_TYPE_NONE)
        return IsEntityVisible(ent, bone);

    WeaponDef* weapDef = BG_GetWeaponDef(cg->predictedPlayerState.weapon);

    BulletFireParams bp;
    BulletTraceResults br;

    bp.weaponEntIndex = 702;
    bp.ignoreEntIndex = cg->predictedPlayerState.clientNum;
    bp.damageMultiplier = 1.0;
    bp.methodOfDeath = weapDef->bRifleBullet + 1;
    bp.origStart = viewOrigin;
    bp.start = viewOrigin;
    bp.end = *bone;

    vec3_t delta, angles;
    delta.x = bone->x - viewOrigin.x;
    delta.y = bone->y - viewOrigin.y;
    delta.z = bone->z - viewOrigin.z;
    vectoangles(&delta, &angles);
    AngleVectors(&angles, &bp.dir, 0, 0);

    if (!CG_ShouldSimulateBulletFire(0, &bp))
        return false;

    SimulateBulletFire(&bp, &br);

    return bp.ignoreEntIndex == ent->nextState.number;
}

void FetchEntityData(centity_t* ent)
{
    int clientNum = (ent->nextState.clientNum >> 8) & 0xF;
    auto& target = targets[clientNum];

    // Positions
    DObj* obj = Com_GetClientDObj(ent->nextState.number, ent->pose.localClientNum);

    if (obj && obj->models && obj->skel.mat)
    {
        target.origin = ent->pose.origin;
        target.angles = ent->pose.angles;
        target.mins = obj->models[0]->mins;
        target.maxs = obj->models[0]->maxs;
        target.distance = cg->snap->ps.origin.distance(target.origin);

        for (int i = 0; i < 16; i++)
        {
            int skelBoneIndex = GetBoneIndex(obj, j_bones[i]);
            target.bones[i] = obj->skel.mat[skelBoneIndex].trans;
        }
    }

    // States
    vec3_t boneAimed = target.bones[Config->AimBoneIndex];

    target.alive = IsEntityAlive(ent);
    target.enemy = IsEntityEnemy(ent);

    if (target.alive && target.enemy)
    {
        target.visible = IsEntityVisible(ent, &boneAimed);
        target.hittable = IsEntityHittable(ent, &boneAimed);
    }
    else
    {
        target.visible = false;
        target.hittable = false;
    }
}

bool AreGameStructuresReady()
{
    return centity && cg && cg->snap && client;
}

bool AreWeAlive()
{
    if (!AreGameStructuresReady() || !Live_IsUserInGame(0))
        return false;

    centity_t* self = &centity[cg->clientNum];
    return IsEntityAlive(self);
}

void FetchAllPlayersInformations()
{
    if (!AreWeAlive())
        return;

    //cg->bThirdPerson = true;

    for (int i = 0; i < 12; i++)
    {
        centity_t* ent = &centity[i];
        int clientNum = (ent->nextState.clientNum >> 8) & 0xF;

        if (clientNum == cg->clientNum)
        {
            CG_GetPlayerViewOrigin(0, &cg->predictedPlayerState, &viewOrigin);
            continue;
        }

        FetchEntityData(ent);
    }

    char buffer[512];
    snprintf(buffer, 512, 
        "target: %i\n"
        "viewOrigin: %f %f %f\n"
        "visible: %i\n"
        "alive: %i\n"
        "enemy: %i\n", 
        currentTarget, viewOrigin.x, viewOrigin.y, viewOrigin.z,
        targets[1].visible, targets[1].alive, targets[1].enemy);

    DrawText(buffer, 20, 30, 50, { 1.0 }, 1.0, 0.0, normalFont);
}

bool IsTargetValid(TargetInfo& target)
{
    if (!target.alive || !target.enemy)
        return false;

    switch (Config->AimbotVisibility)
    {
    case(0): return true;
    case(1): return target.visible;
    case(2): return target.hittable;
    default: return false;
    }
}

int FindBestTarget()
{
    float closestDistance = FLT_MAX;
    int closestTarget = -1;

    for (int i = 0; i < cg->snap->numClients; i++)
    {
        if (i == cg->clientNum)
            continue;

        if (IsTargetValid(targets[i]) && (targets[i].distance < closestDistance))
        {
            closestDistance = targets[i].distance;
            closestTarget = i;
        }
    }

    return closestTarget;
}

void CalculateAimingAngles(int targetIndex)
{
    vec3_t bone = targets[targetIndex].bones[Config->AimBoneIndex];
    vec3_t deltaAngles = cg->snap->ps.delta_angles;

    vec3_t direction
    {
        bone.x - viewOrigin.x ,
        bone.y - viewOrigin.y ,
        bone.z - viewOrigin.z
    };
    vectoangles(&direction, &aimAngles);

    aimAngles.x -= deltaAngles.x;
    aimAngles.y -= deltaAngles.y;
    aimAngles.z -= deltaAngles.z;
}

usercmd_s* GetUserCmd(int number)
{
    return &client->cmds[number & 127];
}

void ApplyCmdButtonMasks(usercmd_s* previousCmd, usercmd_s* currentCmd)
{
    int fireMask = UCMD_MASK_FIRE | (BG_IsDualWield(cg->predictedPlayerState.weapon) ? UCMD_MASK_AIM : 0);

    // Fire
    if (Config->FireMacro || (Config->AutoShoot && currentTarget != -1))
        previousCmd->buttonBits[0] &= fireMask;

    if (Config->AutoShoot && currentTarget != -1)
        currentCmd->buttonBits[0] |= fireMask;

    // Jump
    if (Config->JumpMacro)
        previousCmd->buttonBits[0] &= UCMD_MASK_JUMP;
}

int ConvertAngleToShort(float angle)
{
    // From https://github.com/id-Software/Quake-III-Arena/blob/master/code/game/q_shared.h#L1082
    return static_cast<int>(angle * (65536.0 / 360.0)) & 0xFFFF;
}

void GetBulletEndPos(unsigned int* randSeed, float f1, float spread, const vec3_t* start, vec3_t* end, vec3_t* dir, const vec3_t* forwardDir, const vec3_t* rightDir, const vec3_t* upDir, float minAngle, float maxAngle, float maxRange, int weapon, int shotIndex, int maxShotIndex)
{
    // How the fuck does that work
    int stack = (int)__builtin_frame_address();
    *(int*)(stack + 0x74) = (int)(forwardDir);
    *(int*)(stack + 0x7C) = (int)(rightDir);
    *(int*)(stack + 0x84) = (int)(upDir);
    *(int*)(stack + 0x90) = (int)(weapon);
    *(int*)(stack + 0x9C) = (int)(shotIndex);
    *(int*)(stack + 0xA4) = (int)(maxShotIndex);
    hook::invoke<void>(0x1337, randSeed, f1, spread, start, end, dir, minAngle, maxAngle, maxRange);
}

float GetCurrentMaxRange()
{
    int weapon = cg->predictedPlayerState.weapon;
    WeaponDef* weapDef = BG_GetWeaponDef(weapon);

    if (weapDef->weapClass == WEAPCLASS_SPREAD || weapDef->weapClass == WEAPCLASS_PISTOL_SPREAD)
    {
        float v147 = hook::invoke<float>(0x1337, weapon);
        float v148 = hook::invoke<float>(0x1337, weapon);

        return __builtin_fsels(v147 - v148, v147, v148);
    }

    return sv_bullet_range->current.value;
}

void PredictSpreadDirection(vec3_t* spreadAngles)
{
    // Calculate current spread
    float minSpread, maxSpread, adsSpread, currentSpread;
    bool usingAds = cg->predictedPlayerState.fWeaponPosFrac == 1.0;

    BG_GetSpreadForWeapon(&cg->predictedPlayerState, cg->predictedPlayerState.weapon, &minSpread, &maxSpread);
    adsSpread = BG_GetWeaponDef(cg->predictedPlayerState.weapon)->fAdsSpread;

    currentSpread = Lerp<float>(usingAds ? adsSpread : minSpread, maxSpread, cg->aimSpreadScale / 255.0);

    // Get view directions
    vec3_t viewAxis[3], end, dir;
    CG_GetPlayerViewDirection(0, &cg->predictedPlayerState, &viewAxis[0], &viewAxis[1], &viewAxis[2]);

    // Get bullet direction
    unsigned int seed = cg->predictedPlayerState.commandTime;
    BG_seedRandWithGameTime(&seed);
    GetBulletEndPos(&seed, 0.0, currentSpread, &viewOrigin, &end, &dir, &viewAxis[0], &viewAxis[1], &viewAxis[2], 0.0, 360.0, GetCurrentMaxRange(), cg->predictedPlayerState.weapon, 0, 1);

    vectoangles(&dir, spreadAngles);
    spreadAngles->x = cg->gunPitch - spreadAngles->x;
    spreadAngles->y = cg->gunYaw - spreadAngles->y;
}

void ApplyAngles(usercmd_s* cmd)
{
    switch (Config->AimType)
    {
    case(0):
        client->viewAngles = aimAngles;
        break;
    case(1):
        cmd->angles[0] = ConvertAngleToShort(aimAngles.x);
        cmd->angles[1] = ConvertAngleToShort(aimAngles.y);
        break;
    }

    if (Config->NoSpread)
    {
        vec3_t spreadAngles;
        PredictSpreadDirection(&spreadAngles);

        cmd->angles[0] += ConvertAngleToShort(spreadAngles.x);
        cmd->angles[1] += ConvertAngleToShort(spreadAngles.y);
    }
}

void CL_SendCmd_Hook(CallContext_t& ctx)
{
    // .text:0x1337 F8 01 00 A0 std       r0, 0x90+arg_10(r1) # Store Double Word
    uint64_t* stackFrame = ctx.GetGpr<uint64_t*>(1);
    stackFrame[0xA0 / sizeof(uint64_t)] = ctx.r0;

    if (!AreWeAlive())
        return;

    usercmd_s* previousCmd = GetUserCmd(client->cmdNumber - 1);
    usercmd_s* currentCmd = GetUserCmd(client->cmdNumber);
    usercmd_s* nextCmd = GetUserCmd(client->cmdNumber + 1);

    previousCmd->serverTime++;
    nextCmd->serverTime--;

    currentTarget = FindBestTarget();

    ApplyCmdButtonMasks(previousCmd, currentCmd);

    if (currentTarget == -1)
        return;

    CalculateAimingAngles(currentTarget);
    ApplyAngles(previousCmd);
}

namespace aim_target
{
    void start()
    {
        scheduler::loop(scheduler::renderer, FetchAllPlayersInformations, 0);
        hook::inject(0x1337, CL_SendCmd_Hook);
    }
}


/*
    plan:
     - [X] add basic aimbot
     - [X] make it work in 3rd person
     - [X] add silent aimbot
     - [X] add no spread
     - [ ] fix movement
     - [X] auto wall
     - [ ] prediction
     - [ ] aim key
     - [ ] add different aiming type like legit, lock-on, etc
     - [ ] idk
*/