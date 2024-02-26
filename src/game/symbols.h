
namespace
{
    /***************************************************************
     * Functions
     **************************************************************/

    // Common
    static symbol<int(const char* fmt, ...)> printf{ 0x1337 };
    static symbol<int(char* buffer, size_t s, const char* fmt, ...)> snprintf{ 0x1337 };
    static symbol<float(float x, float y)> fmodf{ 0x1337 };
    static symbol<float(float x)> tanf{ 0x1337 };
    static symbol<double(double x, int r4)> _FSin{ 0x1337 };

    static symbol<int()> Sys_Milliseconds{ 0x1337 };

    static symbol<const char* (const char* str1, const char* str2)> strstr{ 0x1337 };
    static symbol<int(const char* str1, const char* str2)> I_stricmp{ 0x1337 };

    // Lua
    static symbol<void(int localClientNum, const wchar_t* title, const wchar_t* initText, int maxLength, void(*callback)(int localClientNum, const wchar_t* text, int maxLength, bool cancelled), int oskPanelFlag)> SpawnKeyboard{ 0x1337 };
    static symbol<void(int localClientNum, const char* message, bool anyControllerAllowed)> ShowNoticeDialog{ 0x1337 };
    static symbol<void(int localClientNum, const char* message, bool anyControllerAllowed)> ShowWarningDialog{ 0x1337 };
    static symbol<void(int localClientNum, const char* message, bool anyControllerAllowed)> ShowErrorDialog{ 0x1337 };

    static symbol<bool(const char* name, Font** handle)> LUI_Interface_RegisterFont{ 0x1337 };
    static symbol<bool(const char* name, Material** handle)> LUI_Interface_RegisterMaterial{ 0x1337 };
    static symbol<bool(const char* name, GfxImage** handle)> LUI_Interface_RegisterImage{ 0x1337 };

    static symbol<bool(int controllerIndex)> Live_IsUserInGame{ 0x1337 };

    // UI
    static symbol<ScreenPlacement* (int localClientNum)> ScrPlace_GetView{ 0x1EC090 };
    static symbol<void(const ScreenPlacement* scrPlace, float x, float y, float width, float height, float angle, const vec4_t* color, Material* material)> CG_DrawRotatedPicPhysical{ 0x1337 };
    static symbol<void* (const char* text, int maxChars, Font* font, float x, float y, float xScale, float yScale, float rotation, const vec4_t* color, int style)> R_AddCmdDrawTextInternal{ 0x1337 };
    static symbol<int(int localClientNum, const char* text, int maxChars, Font* font)> R_TextWidth{ 0x1337 };

    // Client
    static symbol<unsigned int(const char* str)> SL_FindString{ 0x1337 };
    static symbol<const char* (unsigned int stringValue)> SL_ConvertToString{ 0x1337 };
    static symbol<DObj* (int handle, int localClientNum)> Com_GetClientDObj{ 0x1337 };

    static symbol<void(int localClientNum, const playerState_s* ps, vec3_t* forward, vec3_t* right, vec3_t* up)> CG_GetPlayerViewDirection{ 0x1337 };
    static symbol<bool(int localClientNum, const playerState_s* ps, vec3_t* origin)> CG_GetPlayerViewOrigin{ 0x1337 };
    static symbol<bool(int localClientNum, const centity_t* entity)> CG_IsEntityFriendlyNotEnemy{ 0x1337 };
    static symbol<void(trace_t* results, vec3_t* start, vec3_t* end, int passEntityNum, int contentMask, bool checkRopes, void* context)> CG_LocationalTrace{ 0x1337 };
    static symbol<void(unsigned int* randSeed, float spread, const vec3_t* start, vec3_t* end, vec3_t* dir, const vec3_t* forwardDir, const vec3_t* rightDir, const vec3_t* upDir, const float maxRange, int weapon, int shotIndex, int maxShotIndex)> CG_BulletEndpos{ 0x1337 };

    static symbol<bool(int localClientNum, int clientNum, int perkIndex)> CG_ClientHasPerk{ 0x1337 };
    static symbol<bool(int localClientNum, BulletFireParams* bp)> CG_ShouldSimulateBulletFire{ 0x1337 };
    static symbol<void(int localClientNum, BulletFireParams* bp, BulletTraceResults* br, int weapon, int attacker, const vec3_t* start, bool r9, bool r10, int penetrateType)> CG_SimulateBulletFire_Internal{ 0x1337 };

    // Weapon
    static symbol<void(unsigned int* pHoldRand)> BG_seedRandWithGameTime{ 0x1337 };
    static symbol<float(unsigned int* pHoldRand)> BG_random{ 0x1337 };

    static symbol<WeaponDef* (int weapon)> BG_GetWeaponDef{ 0x1337 };
    static symbol<WeaponVariantDef* (int weapon)> BG_GetWeaponVariantDef{ 0x1337 };
    static symbol<void(const playerState_s* ps, int weapon, float* minSpread, float* maxSpread)> BG_GetSpreadForWeapon{ 0x1337 };
    static symbol<bool(int weapon, int perk)> BG_WeaponHasPerk{ 0x1337 };
    static symbol<bool(int weapon)> BG_IsDualWield{ 0x1337 };

    static symbol<int(int weapon)> BG_GetPenetrateType{ 0x1337 };
    static symbol<float(int penetrateType, int surfaceType)> BG_GetSurfacePenetrationDepth{ 0x1337 };
    static symbol<bool(BulletFireParams* bp, BulletTraceResults* br, float dist)> BG_AdvanceTrace{ 0x1337 };
    static symbol<bool(int localClientNum, BulletFireParams* bp, BulletTraceResults* br, int weapon, int attacker, int lastSurfaceType)> BulletTrace{ 0x1337 };

    // World
    static symbol<bool(int localClientNum, vec3_t* worldPos, vec2_t* screenPos)> WorldPosToScrenPos{ 0x1337 };
    static symbol<void(const vec3_t* vec, vec3_t* angles)> vectoangles{ 0x1337 };
    static symbol<void(const vec3_t* angles, vec3_t* forward, vec3_t* right, vec3_t* up)> AngleVectors{ 0x1337 };
    static symbol<float(float angle)> AngleNormalize360{ 0x1337 };
    static symbol<void(const vec3_t* angles, vec3_t* axis)> AnglesToAxis{ 0x1337 };
    static symbol<void(const vec3_t* axis, vec3_t* angles)> AxisToAngles{ 0x1337 };

    // Server
    static symbol<int(int min, int max)> G_irand{ 0x1337 };

    /***************************************************************
     * Variables
     **************************************************************/

    static bool& g_oskRunning = *reinterpret_cast<bool*>(0x1337);

    static dvar_t* (&cg_875C84B7) = *reinterpret_cast<dvar_t**>(0x1337);
    static dvar_t* (&sv_bullet_range) = *reinterpret_cast<dvar_t**>(0x1337);
    static dvar_t* (&sv_penetrationCount) = *reinterpret_cast<dvar_t**>(0x1337);
    static dvar_t* (&perk_bulletPenetrationMultiplier) = *reinterpret_cast<dvar_t**>(0x1337);

    static cg_t* (&cg) = *reinterpret_cast<cg_t**>(0x1337);
    static centity_t* (&centity) = *reinterpret_cast<centity_t**>(0x1337);
    static clientActive_t* (&client) = *reinterpret_cast<clientActive_t**>(0x1337);
}