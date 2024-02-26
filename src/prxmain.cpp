
#include "globals.h"
#include "components/scheduler.h"
#include "components/patches.h"
#include "components/aim_assist/aim_target.h"
#include "components/bgame/bg_pmove.h"
#include "components/bgame/bg_weapons.h"
#include "components/bgame/bg_weapons_ammo.h"
#include "components/cgame/cg_camera.h"
#include "components/cgame/cg_weapons.h"
#include "components/database/db_registry.h"
#include "components/gfx_3d/r_rendercmds.h"
#include "components/gfx_3d/rb_backend.h"
#include "components/ps3/ps3_gamepad.h"
#include "components/server/sv_client.h"
#include "game/game.h"
#include "gui/offhost.h"
#include "gui/onhost.h"
#include "utils/hooking.h"
#include "utils/imports.h"
#include <sys/prx.h>

#pragma diag_suppress 77
SYS_MODULE_INFO(Elysium, 0, ELYSIUM_VERSION_MAJOR, ELYSIUM_VERSION_MINOR);
SYS_MODULE_START(module_start);
SYS_MODULE_STOP(module_stop);

extern "C"
{
    int module_start()
    {
        Config = new Configuration();
        Offhost = new OffhostMenu();
        Onhost = new OnhostMenu();

        hook::start(init_proc, 50);
        scheduler::start();
        patches::start();

        aim_target::start();
        bg_pmove::start();
        bg_weapons::start();
        bg_weapons_ammo::start();
        cg_camera::start();
        cg_weapons::start();
        db_registry::start();
        ps3_gamepad::start();
        r_rendercmds::start();
        rb_backend::start();
        sv_client::start();

        scheduler::loop(scheduler::renderer, []
            {
                Offhost->Run();
            }, 0);

        printf("Hook count: %i\n", hook::count());

        return 0;
    }

    int module_stop()
    {
        hook::stop();
        scheduler::stop();

        delete Onhost;
        delete Offhost;
        delete Config;
        return 0;
    }
}