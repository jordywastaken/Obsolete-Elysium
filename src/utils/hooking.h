
#pragma once

#include <sys/sys_types.h>

struct CallContext_t
{
    uint64_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r30, r31;
    uint64_t lr, ctr, cr, xer;
    double f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31;

    template<typename T>
    inline T& GetGpr(int index)
    {
        union reg_t
        {
            uint64_t packed;
            struct
            {
                char ___padding[sizeof(uint64_t) - sizeof(T)];
                T value;
            };
        };

        reg_t& reg = *(reg_t*)(&r0 + index);
        return reg.value;
    }

    inline double& GetFpr(int index)
    {
        return (&f0)[index];
    }
};

struct OPD_t
{
    uintptr_t function, toc;
};

typedef void(*HookCallback_t)(CallContext_t&);

// First sub routine in every application executable on ps3
static constexpr uintptr_t init_proc = 0x10200;

namespace hook
{
    void start(uintptr_t location, size_t entryCount);
    void stop();
    int count();

    void handler(CallContext_t& ctx);
    void inject(uintptr_t address, HookCallback_t callback);

    void jump(uintptr_t address, uintptr_t callback);
    void branch(uintptr_t address, uintptr_t destination, bool linked);
    void nop(uintptr_t address, size_t instructionCount);
    void copy(uintptr_t address, uint32_t* instructions, size_t instructionCount);

    uintptr_t toc();

    template<typename T, typename... Args>
    static inline T invoke(OPD_t* funcDescriptor, Args... args)
    {
        auto func = reinterpret_cast<T(*)(Args...)>(funcDescriptor);
        return func(args...);
    }

    template<typename T, typename... Args>
    static inline T invoke(uintptr_t address, Args... args)
    {
        volatile OPD_t func_fptr{ address, toc() };
        auto func = reinterpret_cast<T(*)(Args...)>(&func_fptr);

        return func(args...);
    }
}