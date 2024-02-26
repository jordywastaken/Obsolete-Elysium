
#pragma once

enum MessageDialogType
{
    MESSAGE_DIALOG_TYPE_INFO,
    MESSAGE_DIALOG_TYPE_WARNING,
    MESSAGE_DIALOG_TYPE_ERROR,
    MESSAGE_DIALOG_TYPE_NOTICE
};

enum DvarSetSource
{
    DVAR_SOURCE_INTERNAL = 0,
    DVAR_SOURCE_EXTERNAL = 1,
    DVAR_SOURCE_SCRIPT = 2
};

enum dvarFlags_t
{
    DVAR_ARCHIVE = (1 << 0),			
    DVAR_USERINFO = (1 << 1),			
    DVAR_SERVERINFO = (1 << 2),			
    DVAR_SYSTEMINFO = (1 << 3),			
    DVAR_INIT = (1 << 4),				
    DVAR_LATCH = (1 << 5),				
    DVAR_ROM = (1 << 6),				
    DVAR_CHEAT = (1 << 7),				
    DVAR_DEVELOPER = (1 << 8),			
    DVAR_SAVED = (1 << 9),				
    DVAR_NORESTART = (1 << 10),			
    DVAR_CHANGEABLE_RESET = (1 << 12),	
    DVAR_EXTERNAL = (1 << 14),			
    DVAR_AUTOEXEC = (1 << 15)			
};

enum dvarType_t
{
    DVAR_TYPE_INVALID = 0x0,
    DVAR_TYPE_BOOL = 0x1,
    DVAR_TYPE_FLOAT = 0x2,
    DVAR_TYPE_FLOAT_2 = 0x3,
    DVAR_TYPE_FLOAT_3 = 0x4,
    DVAR_TYPE_FLOAT_4 = 0x5,
    DVAR_TYPE_INT = 0x6,
    DVAR_TYPE_ENUM = 0x7,
    DVAR_TYPE_STRING = 0x8,
    DVAR_TYPE_COLOR = 0x9,
    DVAR_TYPE_INT64 = 0xA,
    DVAR_TYPE_UINT64 = 0xB,
    DVAR_TYPE_LINEAR_COLOR_RGB = 0xC,
    DVAR_TYPE_COLOR_XYZ = 0xD,
    DVAR_TYPE_COLOR_LAB = 0xE,
    DVAR_TYPE_COUNT = 0xF
};

enum entityType_t
{
    ET_GENERAL,
    ET_PLAYER,
    ET_PLAYER_CORPSE,
    ET_ITEM,
    ET_MISSILE,
    ET_PLAYER_INVISIBLE,
    ET_SCRIPTMOVER,
    ET_SOUND_BLEND,
    ET_FX,
    ET_LOOP_FX,
    ET_PRIMARY_LIGHT,
    ET_LENSFLARE,
    ET_REFLECTION_PROBE,
    ET_HELICOPTER,
    ET_PLANE,
    ET_VEHICLE,
    ET_VEHICLE_SPAWNER,
    ET_VEHICLE_CORPSE,
    ET_ACTOR,
    ET_ACTOR_SPAWNER,
    ET_ACTOR_CORPSE,
    ET_STREAMER_HINT,
    ET_ZBARRIER,
    ET_TRIGGER,
};

enum entityEvent_t
{
	EV_NONE,
	EV_FOLIAGE_SOUND,
	EV_STOP_WEAPON_SOUND,
	EV_STOP_SOUND_ALIAS,
	EV_SOUND_ALIAS,
	EV_SOUND_ALIAS_NOTIFY,
	EV_SOUND_BATTLECHAT_ALIAS,
	EV_STOPSOUNDS,
	EV_ITEM_PICKUP,
	EV_AMMO_PICKUP,
	EV_NOAMMO,
	EV_NOAMMO_LEFT,
	EV_EMPTY_OFFHAND,
	EV_RESET_ADS,
	EV_RELOAD,
	EV_RELOAD_FROM_EMPTY,
	EV_RELOAD_START,
	EV_RELOAD_END,
	EV_RELOAD_START_NOTIFY,
	EV_RELOAD_ADDAMMO,
	EV_RAISE_WEAPON,
	EV_FIRST_RAISE_WEAPON,
	EV_RAISE_PICKUP_WEAPON,
	EV_PUTAWAY_WEAPON,
	EV_WEAPON_ALT,
	EV_WEAPON_SWITCH_STARTED,
	EV_PULLBACK_WEAPON,
	EV_HOLDFIRE_WEAPON,
	EV_FIRE_WEAPON_DELAY_START,
	EV_FIRE_WEAPON_SCRIPTED,
	EV_FIRE_WEAPON,
	EV_FIRE_WEAPON_LASTSHOT,
	EV_FIRE_WEAPON_LEFT,
	EV_FIRE_WEAPON_LASTSHOT_LEFT,
	EV_FIRE_WEAPON_BALL_PASS,
	EV_FIRE_RICOCHET,
	EV_FIRE_WEAPON_GRAPPLE,
	EV_RECHAMBER_WEAPON,
	EV_EJECT_BRASS,
	EV_FIRE_MELEE_SWIPE,
	EV_FIRE_MELEE_POWER,
	EV_FIRE_MELEE_POWER_LEFT,
	EV_FIRE_MELEE,
	EV_MELEE_JUKE,
	EV_MELEE_JUKE_END,
	EV_MELEE_LUNGE,
	EV_MELEE_STRUGGLE,
	EV_MELEE_COUNTER,
	EV_FIRE_MELEE_DELAYED,
	EV_MELEE_HIT,
	EV_MELEE_HIT_ENT,
	EV_MELEE_HIT_FROM_BEHIND,
	EV_MELEE_MISS,
	EV_MELEE_BLOOD,
	EV_WEAPON_DEPLOYING,
	EV_WEAPON_FINISH_DEPLOYING,
	EV_WEAPON_FINISH_BREAKING_DOWN,
	EV_INC_CHARGESHOT_LEVEL,
	EV_HOLDFIRE_CANCEL,
	EV_HOLDFIRE_FIRED,
	EV_PREP_OFFHAND,
	EV_USE_OFFHAND,
	EV_SWITCH_OFFHAND,
	EV_PREP_RIOTSHIELD,
	EV_DEPLOY_RIOTSHIELD,
	EV_LOWER_RIOTSHIELD,
	EV_FIRE_BARREL_1,
	EV_FIRE_BARREL_2,
	EV_FIRE_BARREL_3,
	EV_FIRE_BARREL_4,
	EV_BULLET_HIT,
	EV_BULLET_HIT_SHIELD,
	EV_BULLET_HIT_CLIENT_SHIELD,
	EV_EXPLOSIVE_IMPACT_ON_SHIELD,
	EV_EXPLOSIVE_SPLASH_ON_SHIELD,
	EV_BULLET_HIT_CLIENT,
	EV_DESTRUCTIBLE_BULLET_HIT,
	EV_DESTRUCTIBLE_EXPLOSION_HIT,
	EV_GRENADE_BOUNCE,
	EV_GRENADE_EXPLODE,
	EV_ROCKET_EXPLODE,
	EV_ROCKET_EXPLODE_NOMARKS,
	EV_EXPLODE_ON_CLIENT,
	EV_FLASHBANG_EXPLODE,
	EV_CUSTOM_EXPLODE,
	EV_CUSTOM_EXPLODE_NOMARKS,
	EV_SENSOR_GRENADE_EXPLODE,
	EV_CHANGE_TO_DUD,
	EV_DUD_EXPLODE,
	EV_DUD_IMPACT,
	EV_MISSILE_REPULSED,
	EV_FIRE_EXPLODE,
	EV_TIMED_FX,
	EV_MOLOTOV_FLOAT,
	EV_FAKE_FIRE,
	EV_PLAY_FX,
	EV_PLAY_FX_IGNORE_PAUSE,
	EV_PLAY_FX_ON_TAG,
	EV_PLAY_FX_ON_TAG_IGNORE_PAUSE,
	EV_PLAY_FX_ON_CAMERA,
	EV_PLAY_FX_ON_CAMERA_IGNORE_PAUSE,
	EV_PHYS_EXPLOSION_SPHERE,
	EV_PHYS_EXPLOSION_CYLINDER,
	EV_PHYS_EXPLOSION_JOLT,
	EV_PHYS_JET_THRUST,
	EV_PHYS_LAUNCH,
	EV_BALLISTIC_TARGET,
	EV_CREATE_DYNENT,
	EV_CREATE_ROPE,
	EV_DETACH_ENTITY,
	EV_DELETE_ROPE,
	EV_ROPE_COLLIDE_WORLD,
	EV_ROPE_COLLIDE_ENT,
	EV_ROPE_SETFLAG,
	EV_ROPE_SETPARAM,
	EV_ROPE_ADDANCHOR,
	EV_ROPE_REMOVEANCHOR,
	EV_ROPE_MOVEANCHOR,
	EV_EARTHQUAKE,
	EV_SCREEN_SHAKE,
	EV_GRENADE_DROP,
	EV_GRENADE_SUICIDE,
	EV_DETONATE,
	EV_DETONATE_OFFHAND,
	EV_MISSILE_REMOTE_BOOST,
	EV_MISSILE_REMOTE_BRAKE,
	EV_PLAY_RUMBLE_ON_ENT,
	EV_PLAY_RUMBLE_ON_POS,
	EV_PLAY_RUMBLELOOP_ON_ENT,
	EV_PLAY_RUMBLELOOP_ON_POS,
	EV_STOP_RUMBLE,
	EV_STOP_ALL_RUMBLES,
	EV_OBITUARY,
	EV_REVIVE_OBITUARY,
	EV_DIRECTIONAL_HIT_INDICATOR,
	EV_ANIMATE_UI,
	EV_KILLSTREAK_DAMAGE,
	EV_ROUND_START_DELAY_HINT,
	EV_NO_FRAG_GRENADE_HINT,
	EV_NO_SPECIAL_GRENADE_HINT,
	EV_GRENADE_NOT_ALLOWED_HINT,
	EV_NO_TARGET_IN_RANGE_HINT,
	EV_TARGET_TOO_CLOSE_HINT,
	EV_TARGET_NOT_ENOUGH_CLEARANCE,
	EV_LOCKON_REQUIRED_HINT,
	EV_FOOTSTEP_SPRINT,
	EV_FOOTSTEP_RUN,
	EV_FOOTSTEP_WALK,
	EV_FOOTSTEP_CROUCHRUN,
	EV_FOOTSTEP_CROUCHWALK,
	EV_FOOTSTEP_PRONE,
	EV_LADDER_HAND_GRAB,
	EV_MANTLE,
	EV_JUMP,
	EV_LEAP,
	EV_TRM,
	EV_WALLRUN_START_LEFT,
	EV_WALLRUN_START_RIGHT,
	EV_WALLRUN_FALL_START,
	EV_WALLRUN_JUMP,
	EV_WALLRUN_END,
	EV_WALLRUN_FALL_END,
	EV_DOUBLEJUMP_DISABLED,
	EV_DOUBLEJUMP_BOOST,
	EV_DOUBLEJUMP_FULLENERGY_BOOST,
	EV_SLIDE_START,
	EV_SOFT_LANDING,
	EV_MEDIUM_LANDING,
	EV_HARD_LANDING,
	EV_DAMAGE_LANDING,
	EV_WATER_LANDING,
	EV_FIRE_VEHICLE_TURRET,
	EV_FIRE_GUNNER_1,
	EV_FIRE_GUNNER_2,
	EV_FIRE_GUNNER_3,
	EV_FIRE_GUNNER_4,
	EV_FIRE_GUNNER_1A,
	EV_FIRE_GUNNER_2A,
	EV_FIRE_GUNNER_3A,
	EV_FIRE_GUNNER_4A,
	EV_FIRE_GUNNER_1B,
	EV_FIRE_GUNNER_2B,
	EV_FIRE_GUNNER_3B,
	EV_FIRE_GUNNER_4B,
	EV_FIRE_GUNNER_1C,
	EV_FIRE_GUNNER_2C,
	EV_FIRE_GUNNER_3C,
	EV_FIRE_GUNNER_4C,
	EV_START_CAMERA_TWEEN,
	EV_START_CAMERA_TWEEN_QUICK_STOP,
	EV_DESTRUCTIBLE_DISABLE_PIECES,
	EV_FOOTPRINT,
	EV_CANNOTPLANT,
	EV_CANT_EQUIP_PRONE,
	EV_CANT_JUKE,
	EV_CANT_SLAM,
	EV_DTP_LAUNCH,
	EV_DTP_LAND,
	EV_LEAP_LAND,
	EV_SCOPE_ZOOM,
	EV_JAM_WEAPON,
	EV_STACKFIRE,
	EV_MISSILE_STICK,
	EV_BOLT_IMPALE,
	EV_PLAY_WEAPON_DEATH_EFFECTS,
	EV_PLAY_WEAPON_DAMAGE_EFFECTS,
	EV_FACE_EVENT,
	EV_SETLOCALWIND,
	EV_FLOAT_LONGER,
	EV_FORCE_BUOYANCY,
	EV_DISABLE_DEPTH_BUOYANCY_ADJUSTMENTS,
	EV_SCALE_BUOYANCY,
	EV_GIB,
	EV_STANCE_INVALID,
	EV_ENTER_VEHICLE,
	EV_EXIT_VEHICLE,
	EV_IMMEDIATE_RAGDOLL,
	EV_ENT_SPAWNED_SENTINEL,
	EV_PROJECTILE_PASS_THROUGH,
	EV_CTX_COVER_START,
	EV_CTX_COVER_END,
	EV_GADGET_0,
	EV_GADGET_1,
	EV_GADGET_2,
	EV_GADGET_0_DENIAL,
	EV_GADGET_1_DENIAL,
	EV_GADGET_2_DENIAL,
	EV_LOCKON_CHANGE,
	EV_TARGETOOR,
	EV_TARGETHIJACKED,
	EV_TARGETNOTVALID,
	EV_NO_TARGETS,
	EV_NO_LOS_TO_TARGET,
	EV_TARGET_DISABLED,
	EV_TARGETING_ABORTED,
	EV_TARGET_ALREADY_TARGETED,
	EV_GRAPPLE_BEAM_ON,
	EV_GRAPPLE_BEAM_OFF,
};

enum TraceHitType
{ 
	TRACE_HITTYPE_NONE, 
	TRACE_HITTYPE_ENTITY, 
	TRACE_HITTYPE_DYNENT_MODEL, 
	TRACE_HITTYPE_DYNENT_BRUSH, 
	TRACE_HITTYPE_GLASS
};

enum UserCmdButtonsMask
{
	/* buttonBits[0] */
	UCMD_MASK_CYCLE = 0x8,
	UCMD_MASK_GRENADE_TACTICAL = 0x10000,
	UCMD_MASK_GRENADE_LETHAL = 0x20000,
	UCMD_MASK_AIM = 0x100080,
	UCMD_MASK_JUMP = 0x200000,
	UCMD_MASK_CROUCH = 0x400000,
	UCMD_MASK_PRONE = 0x800000,
	UCMD_MASK_RELOAD = 0x4000000,
	UCMD_MASK_MELEE = 0x20000000,
	UCMD_MASK_SPRINT = 0x40040000,
	UCMD_MASK_FIRE = 0x80000000,

	UCMD_MASK_LEAN1 = 0x1000000, // dunno yet which is left or right
	UCMD_MASK_LEAN2 = 0x2000000,

	/* buttonBits[1] */
	UCMD_MASK_STANCE = 0x2,
	UCMD_MASK_DPAD_LEFT = 0x8000,
	UCMD_MASK_DPAD_RIGHT = 0x10000,
	UCMD_MASK_DPAD_DOWN = 0x20000,
	UCMD_MASK_DPAD_UP = 0x40000,

	/* buttonBits[2] */
	UCMD_MASK_STANCE2 = 0x8000000,
	UCMD_MASK_CYCLE2 = 0x1000000,
	UCMD_MASK_SPRINT2 = 0x20000000,
	UCMD_MASK_JUMP2 = 0xd0000000,
};

enum weapClass_t
{
	WEAPCLASS_RIFLE,
	WEAPCLASS_MG,
	WEAPCLASS_SMG,
	WEAPCLASS_SPREAD,
	WEAPCLASS_PISTOL,
	WEAPCLASS_GRENADE,
	WEAPCLASS_ROCKETLAUNCHER,
	WEAPCLASS_TURRET,
	WEAPCLASS_NON_PLAYER,
	WEAPCLASS_GAS,
	WEAPCLASS_ITEM,
	WEAPCLASS_MELEE,
	WEAPCLASS_KILLSTREAK_ALT_STORED_WEAPON,
	WEAPCLASS_PISTOL_SPREAD,
	WEAPCLASS_NUM
};

enum PenetrateType
{ 
	PENETRATE_TYPE_NONE, 
	PENETRATE_TYPE_SMALL, 
	PENETRATE_TYPE_MEDIUM, 
	PENETRATE_TYPE_LARGE, 
	PENETRATE_TYPE_COUNT
};