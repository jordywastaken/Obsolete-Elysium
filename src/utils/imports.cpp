
#include "imports.h"

extern "C"
{
    void* memcpy(void* dest, const void* src, size_t size)
    {
        return _sys_memcpy(dest, src, size);
    }

    void* memset(void* m, int c, size_t n)
    {
        return _sys_memset(m, c, n);
    }
}

void* operator new(size_t size)
{
    return _sys_malloc(size);
}

void* operator new(size_t size, size_t align)
{
    return _sys_memalign(align, size);
}

void* operator new[](std::size_t size)
{
    return _sys_malloc(size);
}

void operator delete(void* mem)
{
    _sys_free(mem);
}

void operator delete[](void* mem)
{
    _sys_free(mem);
}