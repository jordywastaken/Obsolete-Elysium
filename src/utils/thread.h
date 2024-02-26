
#pragma once

#include <sys/ppu_thread.h>

class thread
{
public:
    union job
    {
        ///
        /// Exploiting 32-bit pointers mwahahaha
        ///
        uint64_t packed;
        struct
        {
            void(*callback)(void*);
            void* userdata;
        };
    };

    thread() = default;

    thread(void(*callback)(), const char* name = "", int priority = 1000, int stackSize = 2048, int flag = SYS_PPU_THREAD_CREATE_JOINABLE)
    {
        sys_ppu_thread_create(&id, [](uint64_t arg) -> void
            {
                if (arg)
                    reinterpret_cast<void(*)()>((uint32_t)arg)();

                sys_ppu_thread_exit(0);

            }, reinterpret_cast<uint64_t>(callback), priority, stackSize, flag, name);
    }

    thread(void(*callback)(void*), void* userdata, const char* name = "", int priority = 1000, int stackSize = 2048, int flag = SYS_PPU_THREAD_CREATE_JOINABLE)
    {
        job task;
        task.callback = callback;
        task.userdata = userdata;

        sys_ppu_thread_create(&id, [](uint64_t arg) -> void
            {
                job task;
                task.packed = arg;

                if (task.callback)
                    task.callback(task.userdata);

                sys_ppu_thread_exit(0);

            }, task.packed, priority, stackSize, flag, name);
    }

    bool started()
    {
        return (id != SYS_PPU_THREAD_ID_INVALID);
    }

    bool join()
    {
        if (!started())
            return false;

        uint64_t exitCode;
        return sys_ppu_thread_join(id, &exitCode) == CELL_OK;
    }

    void rename(const char* name)
    {
        if (started())
            sys_ppu_thread_rename(id, name);
    }

    sys_ppu_thread_t id = SYS_PPU_THREAD_ID_INVALID;
};