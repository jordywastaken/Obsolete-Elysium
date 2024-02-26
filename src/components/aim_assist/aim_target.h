
#pragma once

#include "game/game.h"

enum TargetBones
{
    TARGET_BONE_HELMET,
    TARGET_BONE_HEAD,
    TARGET_BONE_NECK,
    TARGET_BONE_SPINEUPPER,
    TARGET_BONE_SPINELOWER,
    TARGET_BONE_MAINROOT,
    TARGET_BONE_SHOULDER_LE,
    TARGET_BONE_SHOULDER_RI,
    TARGET_BONE_ELBOW_LE,
    TARGET_BONE_ELBOW_RI,
    TARGET_BONE_WRIST_LE,
    TARGET_BONE_WRIST_RI,
    TARGET_BONE_KNEE_LE,
    TARGET_BONE_KNEE_RI,
    TARGET_BONE_ANKLE_LE,
    TARGET_BONE_ANKLE_RI,

    TARGET_BONE_COUNT
};

struct TargetInfo
{
    bool alive;
    bool enemy;
    bool visible;
    bool hittable;
    float distance;
    vec3_t bones[TARGET_BONE_COUNT];
    vec3_t origin;
    vec3_t angles;
    vec3_t mins;
    vec3_t maxs;
};

extern const char* j_bones[TARGET_BONE_COUNT];
extern int currentTarget;
extern TargetInfo targets[12];
extern vec3_t viewOrigin;
extern vec3_t aimAngles;

namespace aim_target
{
    void start();
}