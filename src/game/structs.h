
#pragma once

class vec2_t
{
public:
    float x, y;

    vec2_t() = default;
    vec2_t(float a)
        : x(a), y(a)
    { }
    vec2_t(float a1, float a2)
        : x(a1), y(a2)
    { }
    vec2_t(float* a)
        : x(a[0]), y(a[1])
    { }
};

class vec3_t
{
public:
    union
    {
        struct { float x, y, z; };
        struct { float r, g, b; };
        struct { float h, s, v; };
    };

    vec3_t() = default;
    vec3_t(float a)
        : x(a), y(a), z(a)
    { }
    vec3_t(float a1, float a2, float a3)
        : x(a1), y(a2), z(a3)
    { }
    vec3_t(float* a)
        : x(a[0]), y(a[1]), z(a[2])
    { }

    float distance(vec3_t from)
    {
        vec3_t delta
        {
            x - from.x,
            y - from.y,
            z - from.z
        };
        return __fsqrts((delta.x * delta.x) + (delta.y * delta.y) + (delta.z * delta.z));
    }
};

class vec4_t
{
public:
    union
    {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
    };

    vec4_t() = default;
    vec4_t(float a)
        : x(a), y(a), z(a), w(a)
    { }
    vec4_t(float a1, float a2, float a3, float a4)
        : x(a1), y(a2), z(a3), w(a4)
    { }
    vec4_t(float* a)
        : x(a[0]), y(a[1]), z(a[2]), w(a[3])
    { }
};

union GfxColor
{
    unsigned int packed;
    struct
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };
};

struct GfxImage
{

};

struct GfxDrawSurfFields
{ 
    uint64_t objectId : 16;
    uint64_t customIndex : 9;
    uint64_t reflectionProbeIndex : 5;
    uint64_t dlightMask : 2;
    uint64_t materialSortedIndex : 12;
    uint64_t primaryLightIndex : 8;
    uint64_t surfType : 4;
    uint64_t prepass : 2;
    uint64_t primarySortKey : 6;
};

union GfxDrawSurf
{ 
    GfxDrawSurfFields fields; 
    uint64_t packed; 
};

struct MaterialInfo
{ 
    const char* name; 
    /*unsigned int gameFlags; 
    unsigned char pad; 
    unsigned char sortKey;
    unsigned char textureAtlasRowCount;
    unsigned char textureAtlasColumnCount;
    GfxDrawSurf drawSurf; 
    unsigned int surfaceTypeBits; 
    unsigned int layeredSurfaceTypes; 
    unsigned short hashIndex; 
    int surfaceFlags; 
    int contents; */
};

struct Material
{ 
    MaterialInfo info; 

    /*unsigned __int8 stateBitsEntry[36]; 
    unsigned __int8 textureCount; 
    unsigned __int8 constantCount; 
    unsigned __int8 stateBitsCount; 
    unsigned __int8 stateFlags; 
    unsigned __int8 cameraRegion; 
    unsigned __int8 probeMipBits; 
    $F046402F7B20C332E3616324B1FE7017 ___u8; 
    MaterialTextureDef* textureTable; 
    $2EAB1C75FF9F93C174985657B10B1534 ___u10; 
    GfxStateBits* stateBitsTable; 
    Material* thermalMaterial; */
}; // sizeof = 0x28

struct Glyph
{ 
    unsigned short letter; 
    char x0; 
    char y0; 
    unsigned char dx; 
    unsigned char pixelWidth;
    unsigned char pixelHeight;
    float s0; 
    float t0; 
    float s1; 
    float t1; 
};

struct KerningPairs
{ 
    unsigned short wFirst; 
    unsigned short wSecond;
    int iKernAmount; 
};

struct Font
{ 
    const char* fontName; 
    int pixelHeight; 
    int isScalingAllowed; 
    int glyphCount; 
    int kerningPairsCount; 
    Material* material; 
    Material* glowMaterial; 
    Glyph* glyphs; 
    KerningPairs* kerningPairs; 
};

union XAssetHeader
{
    Font* font;
    Material* material;
    GfxImage* image;
};

struct XAsset
{
    int type;
    XAssetHeader header;
};

struct XAssetEntry
{
    XAsset asset;
    char zoneIndex;
    bool inUse;
    short nextHash;
    short nextOverride;
    short usageFrame;
};

struct ScreenPlacement
{ 
    vec2_t scaleVirtualToReal; 
    vec2_t scaleVirtualToFull; 
    vec2_t scaleRealToVirtual; 
    vec2_t virtualViewableMin; 
    vec2_t virtualViewableMax; 
    vec2_t virtualTweakableMin; 
    vec2_t virtualTweakableMax; 
    vec2_t realViewportBase; 
    vec2_t realViewportSize; 
    vec2_t realViewportMid; 
    vec2_t realViewableMin; 
    vec2_t realViewableMax; 
    vec2_t realTweakableMin; 
    vec2_t realTweakableMax; 
    vec2_t subScreen;
    float hudSplitscreenScale; 
};

union DvarValue
{
    bool enabled;
    int integer;
    unsigned int unsignedInt;
    int64_t integer64;
    uint64_t unsignedInt64;
    float value;
    float vec[4];
    const char* string;
    char color[4];
};

union DvarLimits
{
    struct {
        int stringCount;
        const char** strings;
    } enumeration;

    struct {
        int min;
        int max;
    } integer;

    struct {
        float min;
        float max;
    } value, vec;

    struct {
        uint64_t min;
        uint64_t max;
    } integer64;
};

struct dvar_t
{
    unsigned int hash;
    unsigned short flags;
    unsigned char type;
    bool modified;
    DvarValue current;
    DvarValue latched;
    DvarValue reset;
    DvarLimits domain;
    dvar_t* next;
    int padding;
};

struct NetField
{ 
    const char* name; 
    int offset; 
    int size; 
    int bits; 
    unsigned char changeHints; 
};

struct LoopSound
{
    int id;
    short fade;
};

struct HandState
{
    int weaponTime;
    int weaponDelay;
    int weaponState;
    int weaponShotCount;
    short weaponAnim;
    int weaponFrameTimeRemainder;
    int __padding;
    int queuedActionState;
};

struct ViewAngleState
{
    int flags;
    int time;
    int startTimeMs;
    int durationMs;
    int easeMode;
    float startAngles[0x1337];
    float goalAngles[2];
};

struct AnimScripted
{
    int time;
    int rate;
    int lerp;
    int goalTime;
    short anim;
};

struct PlayerLens
{
    int lensState;
    float focalLength;
};

struct VehicleState
{
    vec3_t origin;
    vec3_t angles;
    vec3_t velocity;
    vec3_t angVelocity;
    float tilt[2];
    float tiltVelocity[2];
    float targetHeightDelta;
    float lastGroundHeight;
    int entity;
    int flags;
    int __padding;
    int focusEntNum;
    int focusEntTime;
    float focusAngleOffset[0x1337];
    int playerHeightMapsActive;
    int boostTimeLeft;
    vec3_t dogfightDesiredLocation;
    int jitterEndTime[0x1337];
    vec3_t jitterAccel;
    int drivableDuration;
    int drivableEndTime;
    bool weaponOverheating;
    float weaponHeatPercent;
    float weaponLockonPercent;
    int damageMeter;
    int lockedOnByEntNum;
    int weaponWaitDuration;
    int weaponWaitEndTime;
};

struct SprintState
{
    int sprintRestoreDelayStart;
    int lastSprintStart;
    int lastSprintEnd;
    int sprintStartMaxLength;
    int sprintDuration;
    int sprintCooldown;
    bool sprintButtonUpRequired;
    bool sprintRestore;
    char sprintDelay;
    char dir;
};

struct MantleState
{
    float yaw;
    int timer;
    int transIndex;
    int flags;
};

struct TrmState
{
    float yaw;
    float ledgeYaw;
    int timer;
    int flags;
    int animIndex;
    vec3_t moveVec;
    float animTimeScale;
    char __padding[0x1337];
    float startPos[2];
    float clampDistance;
    int doublejumpEndedTime;
};

struct GrappleState
{
    vec3_t startPos;
    int padding;
};

struct SlideState
{
    vec3_t slideAngleOffsets;
    int flags;
    int slideTime;
    int slideOutDuration;
    int subsequentCount;
    int slideType;
};

struct JukeState
{
    int startTime;
    int jukeYaw;
    int flags;
};

struct PlayerGadgetState
{
    int gadgetPlayerStateFlags;
    int gadgetLastUsedTime;
    int gadgetFlickerTime;
    int gadgetPressTime;
    int gadgetActivateTime;
    int gadgetPowerRemaining;
    short gadgetEntNum;
    char gadgetButtonState;
};

struct SlamState
{
    int startTime;
    int slamYaw;
    int flags;
};

struct CybercomState
{
    char abilityFlags[3];
    char upgradeFlags[3];
    char tacticalRigFlags;
    char disableTacticalWheel;
    int activeType;
    bool specialMeleeActive;
    int specialMeleeDelay;
};

struct WallRunState
{
    int startTimeMs;
    int endTimeMs;
    int flags;
    vec3_t normal;
    float startHeight;
    float verticality;
    float rollFrac;
    int timeLastInputReceived;
    int lastShotMs;
};

struct ClimbState
{
    vec3_t normal;
    vec3_t center;
    short flags;
};

struct LeapState
{
    int volumeTimer;
    int state;
};

struct AnimMovement
{
    vec3_t startOrigin;
    vec3_t originErrorNormal;
    int timer;
    float originErrorDistance;
    float startYaw;
    float errorYaw;
    float timeScale;
    int padding;
    int flags;
    int lerpTime;
};

struct XCam
{
    int xcamIndex;
    int xcamStartTime;
    int xcamLerpEndTime;
    int xcamSubCamera;
    vec3_t xcamBundleOrigin;
    vec4_t xcamBundleAnglesQuat;
    bool ignoreProcessingInitialNoteTracks;
};

struct RenderInfo
{
    float sunShadowSplitDistance;
    int lightingState;
    float modelLodBias;
};

struct playerState_s
{
    int clientNum;
    int commandTime;
    int pm_type;
    int bobCycle;
    uint64_t pm_flags;
    uint64_t weapFlags;
    int otherFlags;
    int pm_time;
    LoopSound loopSOund;
    vec3_t origin;
    vec3_t velocity;
    char __padding_0x1337[0x1337];
    HandState handState[0x1337];
    int movementTime;
    int weaponIdleTime;
    int grenadeTimeLeft;
    int throwBackGrenadeTimeLeft;
    int weaponRestrictKickTime;
    char __padding_0x13370x13370x1337[0xC];
    int foliageSoundTime;
    int gravity;
    int leanf;
    int speed;
    vec3_t delta_angles;
    ViewAngleState viewAngleState;
    int moverTimestamp;
    char __padding_0xF0[0x1337];
    int jumpTime;
    float jumpOriginZ;
    int legsTimer;
    int torsoTimer;
    short legsAnim;
    short torsoAnim;
    char __padding_0x108[0x4];
    AnimScripted animScripted;
    int damageTimer;
    int damageDuration;
    int movementDir;
    int eFlags;
    int eFlags2;
    PlayerLens playerLens;
    VehicleState vehicleState;
    short predictableEventSequence;
    short predictableEventSequenceOld;
    int predictableEvents[0x1337];
    int predictableEventParms[0x1337];
    short unpredictableEventSequence;
    short unpredictableEventSequenceOld;
    int unpredictableEvents[0x1337];
    int unpredictableEventParms[0x1337];
    int offHandWeapon;
    int offhandSecondary;
    int offhandPrimary;
    uint64_t renderOptions;
    int attachmentCosmeticVariantIndexes;
    int momentum;
    int weapon;
    int lastStandPrevWeapon;
    int lastWeaponAltModeSwitch;
    int stowedWeapon;
    int meleeWeapon;
    int lastFiredGadget;
    int gadgetTargetResult;
    float fWeaponPosFrac;
    float fMoveSpeedTransitionLerp;
    float fIdleSpeedFromFireTransitionLerp;
    int adsDelayTime;
    int spreadOverride;
    int weaponSpinLerp;
    vec3_t viewangles;
    float viewHeightCurrent;
    int viewHeightLerpTime;
    float viewHeightLerpTarget;
    int viewHeightState;
    int viewHeightStatePrev;
    float viewBobFactor[2];
    float viewAngleClampBase[2];
    float viewAngleClampRange[2];
    int damageEvent;
    float damageProjYaw;
    int damageCount;
    char __padding_0x2D0[0x5B0 - 0x2D0];
    int proneDirection;
    int proneDirectionPitch;
    int proneTorsoPitch;
    int viewlocked;
    vec3_t linkAngles;
    vec3_t groundTiltAngles;
    int cursorHintString;
    int cursorHintWeapon;
    int iCompassPlayerInfo;
    int spyplaneTypeEnabled;
    int satelliteTypeEnabled;
    int spyplaneTime;
    int satelliteTime;
    int locationSelectionInfo;
    int locationSelectionType;
    SprintState sprintState;
    int lastDtpEnd;
    int lastCtxCover;
    float fTorsoPitch;
    float fWaistPitch;
    float holdBreathScale;
    int holdBreathTimer;
    int chargeShotTimer;
    int chargeShotLevel;
    int shotsFiredFromChamber;
    float quickScopeScale;
    int quickScopeTimer;
    int privateClientFields[5];
    int clientFields[5];
    int uiModelFields[6];
    float moveSpeedScaleMultiplier;
    MantleState mantleState;
    TrmState trmState;
    GrappleState grappleState;
    SlideState slideState;
    JukeState jukeState;
    SlamState slamState;
    int impactTime;
    int doubleJumpState;
    int doubleJumpBoostUpDirection;
    int doubleJumpPitchOffset;
    int doubleJumpRollOffset;
    float playerEnergyRatio;
    int playerEnergyTime;
    PlayerGadgetState playerGadgetState[3];
    CybercomState cybercomState;
    WallRunState wallRunState;
    int playerSwimAnimRate;
    ClimbState climbState;
    LeapState leapState;
    AnimMovement animMovement;
    int meleeChargeTime;
    int meleeConstraintPlane[4];
    int meleeStartTime;
    int meleeFlags;
    int blockMeleeUsageTimer;
    int meleeCounterTimer;
    int chainMeleeCounter;
    XCam mainCam;
    int weapLockFlags[5];
    int weapLockedEntnum[5];
    int weapLockTimer;
    int lockOnRequiredMsgSuppressAfterFireTimer;
    int perks[4];
    char bonuscards[3];
    int actionSlotType[4];
    int actionSlotParam[4];
    int inventoryWeapon;
    int inventoryHeldWeapon;
    char vehicleDefIndex;
    int locBlendTime;
    RenderInfo renderInfo;
    char __padding_0x904[0x4];
    float aimSpreadScale;
    int shellshockTime;
    int shellshockDuration;
    int meleeButtStartTime;
    float dofNearStart;
    float dofNearEnd;
    float dofFarStart;
    float dofFarEnd;
    float dofNearBlur;
    float dofFarBlur;
    float dofViewmodelStart;
    float dofViewmodelEnd;
    int lastFootInWaterTime;
    int lastOutWaterTime;
    int adsZoomSelect;
    int adsZoomSelectTime;
    int adsPrevZoomSelect;
    int adsPrevZoomSelectTime;
    int artilleryInboundIconLocation;
    int visionSetLerpRatio;
    char __padding_0x958[0x1337 - 0x958];
    int deltaTime;
    char __padding_0x1160[0x1337 - 0x1160];
    char uiModelQuartets[2][0x18];
    char __padding_0x2D94[0x69420 - 0x2D94];
    short vehicleAnimBoneIndex;
    char __padding_0x2E3A[0x4];
    short throwBackGrenadeOwner;
    short groundEntityNum;
    short moverEntityNum;
    short viewlocked_entNum;
    short cursorHintEntIndex;
    short meleeAttacker;
    short damageYaw;
    short damagePitch;
    short meleeChargeDist;
    short meleeChargeEnt;
    short weapLockedPivotEntnum;
    short killCamEntity;
    short killCamTargetEntity;
    int killCamWeapon;
    int killCamMod;
    char __padding_0x2E60;
    bool bThirdPerson;
    char __padding_0x2E62;
    char vehicleAnimStage;
    char vehicleEntryPoint;
    char corpseIndex;
    char linkFlags;
    char vehiclePos;
    char vehicleType;
    char movementState;
    char moveType;
    char dmgDirection;
    char dmgType;
    char spreadOverrideState;
    char __padding_0x2E6E;
    char viewHeightLerpDown;
    char cursorHint;
    char impactIntensity;
    char meleeServerResult;
    char meleeState;
    char __padding_0x2E74;
    char walkUnderwater;
    char adsZoomLatchState;
    char shellshockIndex;
    char __padding_0x2E78;
    char meleeButtDirection;
    char waterlevel;
    char __padding_0x2E7B;
    char poisoned;
    char __padding_0x2E7D;
    char introShotsFired;
}; // sizeof = 0x2E80

struct cpose_t
{
    short lightingHandle;
    char eType;
    char eTypeUnion;
    char localClientNum;
    bool isRagdoll;
    int ragdollHandle;
    char __padding_0xC[0x28 - 0xC];
    vec3_t origin; // 0x28
    vec3_t angles;
    vec3_t absmin;
    vec3_t absmax;
    char __padding_0x1337[0x1337 - 0x58];
}; // sizeof = 0x1337

struct trajectory_t
{
    char trType;
    short trDuration;
    int trTime;
    vec3_t trBase;
    vec3_t trDelta;
}; // sizeof = 0x1337

struct LerpEntityState
{
    int eFlags;
    int eFlags2;
    char __padding_0x8999999[0x1337];
    trajectory_t pos;
    trajectory_t apos;
    char __padding_0x1337[0x1337 - 0x50];
}; // sizeof = 0x1337

struct entityState_s
{
    int number;
    char __padding_0x4[0x1337];
    LerpEntityState lerp; // 0x8
    char __padding_0xB4[0x1337];
    int weapon; // 0x138
    char __padding_0x13C[0x1337];
    int groundEntityNum; // 0x1337
    char __padding_0x164[0x1337];
    char eType; // 0x1337
    char __padding_0x168[0x1337];
    short clientNum; // 0x1337
    char __padding_0x1337[0x1337];
}; // sizeof = 0x1337

struct centity_t
{
    cpose_t pose;
    LerpEntityState prevState; // 0x1337
    entityState_s nextState; // 0x1337
    short previousEventSequence; // 0x1337
    char __padding_0x1337[0x1337];
}; // sizeof = 0x1337

struct snapshot_s
{
    int snapFlags;
    int ping;
    int serverTime;
    int physicsTime;
    playerState_s ps;
    int numEntities;
    int numClients;
    int numActors;
    char __padding_0x2E9C[0x16380 - 0x2E9C];
}; // sizeof = 0x16380

struct refdef_t
{
    int x;
    int y;
    int width;
    int height;
    char __padding_0x10[0x70];
    float fov_x; // 0x80
    vec3_t vieworg; // 0x84
    char __padding_0x90[0x10];
    vec3_t viewaxis[3]; // 0xA0
    int frametime;
    int time;
    char __padding_0xCC[0x1BD50 - 0xCC];
}; // sizeof = 0x1BD50

struct clientInfo_t
{
    int infoValid;
    int nextValid;
    int clientNum;
    char name[32]; // 0xC 
    char __padding_0x2C[0x14];
    int team; // 0x40;
    char __padding_0x44[0x70 - 0x44];
    char clantag[4]; // 0x70
    char __padding_0x74[0xD8 - 0x74];
    int health; // 0xD8
    char __padding_0xDC[0x1337 - 0xDC];
}; // sizeof = 0x1337

struct cg_t
{
    int clientNum;
    int localClientNum;
    int demoType;
    int cubemapShot;
    int cubemapSize;
    int renderScreen;
    int latestSnapshotNum;
    int latestSnapshotTime;
    snapshot_s* snap;
    snapshot_s* nextSnap;
    char __padding_0xskids[0x1337 - 0x28];
    snapshot_s activeSnapshots[0x1337];
    bool slowMotionModified; // 0x1337
    float frameInterpolation; 
    int frameTime;
    int time;
    int oldTime;
    int physicsTime;
    char __padding_00x1337[0x69];
    bool bThirdPerson; // 0x1337
    bool thirdPersonRelatedVar;
    short renderingThirdPerson;
    int __padding_0x2C7AC;
    playerState_s predictedPlayerState; // 0x1337
    int lastPlayerStateOverride; // 0x1337
    char __padding_0xno[0x4];
    centity_t predictedPlayerEntity; // 0x1337
    char __padding_0xcopy[0x69B50 - 0x42070];
    refdef_t refdef; // 0x1337
    vec3_t refdefViewAngles; // 0x1337
    char __padding_0xpasting[0x69000 - 0x69AC];
    float gunPitch; // 0x1337
    float gunYaw; // 0x1337
    char __padding_0xallowed[0x6905C - 0x69008];
    float compassNorthYaw; // 0x1337
    vec2_t compassNorth; // 0x1337
    Material* compassMapMaterial; // 0x1337
    Material* compassMapFullMaterial; // 0x1337
    Material* compassMapCorruptMaterial; // 0x1337
    Material* compassMapNoiseMaterial; // 0x1337
    Material* compassMapScrambledMaterial; // 0x1337
    vec2_t compassMapUpperLeft; // 0x1337
    vec2_t compassMapWorldSize; // 0x1337
    int compassLastTime; // 0x1337
    float compassYaw; // 0x1337
    int compassSpeed; // 0x1337
    int compassFadeTime; // 0x1337
    char __padding_0xskidssss[0x68410 - 0x1337];
    clientInfo_t clientInfo[12]; // 0x68410
    char __padding_0xskidsssss[0x98D5C - 0x696D0];
    float aimSpreadScale; // 0x98D5C

    // movers; // 0x6BC0C 
    int moversCount; // 0x6C00C
};

struct clSnapshot_t
{
    int valid;
    int snapFlags;
    int serverTime;
    int physicsTime;
    int messageNum;
    int deltaNum;
    int ping;
    int cmdNum;
    playerState_s ps;
    int numEntities;
    int numClients;
    int numActors;
    int parseMatchStateIndex;
    int parseEntitiesIndex;
    int parseClientsIndex;
    int parseActorsIndex;
    int serverCommandNum;
}; // sizeof = 0x2EC0

struct usercmd_s
{
    int serverTime; // 0x0
    int buttonBits[3]; // 0x4
    int angles[3]; // 0x10
    int weapon; // 0x1C
    int offHandWeapon; // 0x20
    int lastWeaponAltModeSwitch; // 0x24
    char forwardmove; // 0x28
    char rightmove; // 0x29
    char upmove; // 0x2A
    char pitchmove; // 0x2B
    char yawmove; // 0x2C
    short meleeChargeEnt; // 0x2E
    short meleeChargeDist; // 0x30
    char __padding_0x32[0x40 - 0x32];
}; // sizeof = 0x40

struct clientActive_t
{
    char __padding_0x0[0x1337];
    clSnapshot_t snap; // 0x1337
    int snapServerTime; // 0x1337
    int oldSnapServerTime;
    bool alwaysFalse;
    int serverTime;
    int oldServerTime;
    int oldFrameServerTime;
    int serverTimeDelta;
    int extrapolatedSnapshot;
    int newSnapshots;
    int serverId; // 0x1337
    char __padding_0x1337[0x1337];
    bool stanceHeld; // 0x1337
    int stance;
    int stancePosition;
    int stanceTime;
    char __padding__0x1337[0x1337];
    bool switchingToInventory; // 0x1337
    char __padding___0x1337[0x1337];
    vec3_t viewAngles; // 0x1337
    char __padding____0x1337[0x1337];
    usercmd_s cmds[128]; // 0x1337
    int cmdNumber; // 0x1337
    // ...
};

struct XBoneInfo
{
    vec3_t bounds[2];
    vec3_t offset;
    float radiusSquared;
    unsigned char collmap;
};

struct XModel
{
    const char* name;
    unsigned short* boneNames;
    char __padding_0x8[0x1337];
    XBoneInfo* boneInfo; // 0x1337
    float radius; // 0x1337
    vec3_t mins; // 0x1337
    vec3_t maxs; // 0x1337
    char __padding_0x1337[0x1337];
    char numBones; // 0x1337
    char numRootBones; // 0x1337
};

struct DSkelPartBits
{
    int anim[5];
    int control[5];
    int skel[5];
};

struct DObjAnimMat
{
    vec4_t quat;
    vec3_t trans;
    float transWeight;
};

struct DSkel
{
    DObjAnimMat* mat;
    //DSkelPartBits partBits;
    //int timeStamp;
};

struct DObj
{
    char __padding0x1337[0x1337];
    XModel** models; // 0x1337
    char __padding__0x1337[0x1337];
    DSkel skel; // 0x2C
    char __padding_0x1337[0x1337];
    short numBones; // 0x1337
    char numModels; // 0x1337
};

struct trace_t
{
    vec4_t normal;				
    float fraction;	
    int sflags;
    char __padding_0x18[0x4];
    int surfaceType;			
    int surfaceFlags; 		
    int hitType; // 0x24		
    short hitId; // 0x28
    short modelIndex;			
    short partName;			
    short boneIndex;		
    short partGroup;		
    bool allsolid;			
    bool startsolid;		
    bool walkable;			
    char __padding_0x2D[0x13];
}; // sizeof = 0x40

struct BulletTraceResults
{
    trace_t trace;			
    int hitEnt;				
    vec3_t hitPos;			
    bool ignoreHitEnt;		
    int surfaceType; // 0x54	
    char __padding_0x58[0x8];
}; // sizeof = 0x60

struct BulletFireParams
{
    int weaponEntIndex;		
    int ignoreEntIndex;			
    float damageMultiplier;	
    int methodOfDeath;		
    vec3_t origStart;		
    vec3_t start;			
    vec3_t end;				
    vec3_t dir;				
    vec3_t unknown;			
}; // sizeof = 0x4C

struct WeaponDef
{
    char __padding_0x0[0x1337];
    int weapClass; // 0x1337
    char __padding_0x40000000000[0x1337];
    bool bRifleBullet; // 0x1337
    char __padding_0x1337[0x13372];
    bool bBulletImpactExplode; // 0x1337
    char __padding_0xgay[0x1337];
    float fAdsSpread; // 0x1337
};

struct WeaponVariantDef
{
    char __padding_0x0[0x1337];
    WeaponDef* def; // 0x1337
};

struct pmove_t
{ 
    playerState_s* ps; 
    usercmd_s cmd; 
    usercmd_s oldcmd; 
    // ...
};

#include <cstddef>
enum
{
    s = sizeof(centity_t),
    o = offsetof(clientActive_t, viewAngles)
};