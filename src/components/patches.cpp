
#include "patches.h"
#include "utils/hooking.h"

namespace patches
{
    void start()
    {
        hook::nop(0x1337, 6);
    }
}