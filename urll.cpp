#include "urll.h"
#ifndef URLL_USE_FUNCTIONAL
    #ifdef _WIN32
        #include <windows.h>
    #else
        #include <dlfcn.h>
    #endif
#endif

void* URLL::dlopen(const char* location, Modes mode) noexcept
{
#ifdef _WIN32

#else
    return ::dlopen(location, static_cast<int>(mode));
#endif
}

void* URLL::dlsym(void* handle, const char* name) noexcept
{
#ifdef _WIN32

#else
    return ::dlsym(handle, name);
#endif
}

int URLL::dlclose(void* handle) noexcept
{
#ifdef _WIN32

#else
    return ::dlclose(handle);
#endif
}

char* URLL::dlerror() noexcept
{
#ifdef _WIN32

#else
    return ::dlerror();
#endif
}