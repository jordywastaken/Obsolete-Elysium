
#include "scheduler.h"
#include "game/game.h"
#include "utils/hooking.h"
#include "utils/thread.h"
#include <sys/timer.h>

namespace scheduler
{
    pipeline* pipelines[count];
    thread asyncThread;
    bool term = false;

    void pipeline::add(void(*handler)(), int interval, bool once)
    {
        for (auto& task : stack)
        {
            if (task.handler)
                continue;

            task.handler = handler;
            task.interval = interval;
            task.lastCall = Sys_Milliseconds();
            task.once = once;
            break;;
        }
    }

    void pipeline::execute()
    {
        int currentTime = Sys_Milliseconds();
        for (auto& task : stack)
        {
            if (!task.handler)
                continue;

            if (currentTime > task.lastCall + task.interval)
            {
                task.handler();
                task.lastCall = currentTime;

                if (task.once)
                {
                    task.handler = 0;
                    task.interval = 0;
                    task.lastCall = 0;
                    task.once = false;
                }
            }
        }
    }

    void start()
    {
        for (auto& entry : pipelines)
            entry = new pipeline();

        // Async
        asyncThread = thread([]
            {
                while (!term)
                {
                    pipelines[async]->execute();
                    sys_timer_usleep(1337);
                }

            }, "Async Scheduler", 420, 8 * 1024);

        // G_RunFrame
        hook::inject(0x1337, [](CallContext_t& ctx)
            {
                pipelines[server]->execute();
            });

        // R_EndFrame
        hook::inject(0x1337, [](CallContext_t& ctx)
            {
                pipelines[renderer]->execute();
            });

        // R_SetDepthOfField
        hook::inject(0x1337, [](CallContext_t& ctx)
            {
                pipelines[hud]->execute();
            });
    }

    void stop()
    {
        term = true;
        asyncThread.join();

        for (auto& entry : pipelines)
            delete entry, entry = 0;
    }

    void loop(pipeline_t thread, void(*handler)(), int interval)
    {
        pipelines[thread]->add(handler, interval, false);
    }

    void once(pipeline_t thread, void(*handler)(), int interval)
    {
        pipelines[thread]->add(handler, interval, true);
    }
}