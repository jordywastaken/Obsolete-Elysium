
#include "sv_client.h"
#include "game/game.h"
#include "utils/hooking.h"

const char* botNames[]
{
    "bot 1",
    "bot 2",
    "bot 3",
    "bot 4",
    "bot 5",
    "bot 6",
    "bot 7",
    "bot 8",
    "bot 9",
    "bot 10",
    "bot 12",
    "bot 13",
    "bot 14",
    "bot 15",
    "bot 16",
    "bot 17",
    "bot 18"
};
constexpr int nameCount = sizeof(botNames) / sizeof(*botNames);
int nameIndex = -1;

const char* SV_BotNameRandom()
{
    if (nameIndex == -1)
    {
        nameIndex = G_irand(0, nameCount);
    }
    else
    {
        nameIndex++;

        if (nameIndex >= nameCount)
            nameIndex = 0;
    }
    return botNames[nameIndex];
}

namespace sv_client
{
    void start()
    {
        hook::inject(0x1337, [](CallContext_t& ctx)
            {
                ctx.GetGpr<const char*>(4) = "connect \"\\invited\\1\\cg_predictItems\\1\\cl_anonymous\\0\\color\\4\\head\\default\\model\\multi\\snaps\\20\\rate\\5000\\name\\%s\\clanAbbrev\\Bot\\xuid\\%s\\xnaddr\\%s\\natType\\2\\protocol\\%d\\netfieldchk\\%d\\sessionmode\\%s\\qport\\%d\"";
                ctx.GetGpr<const char*>(5) = SV_BotNameRandom();
            });
    }
}