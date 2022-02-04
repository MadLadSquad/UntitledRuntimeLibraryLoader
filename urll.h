#pragma once
#define URLL_USE_FUNCTIONAL

// Define this if you want to use std::function for easier function handling!
#ifdef URLL_USE_FUNCTIONAL
    #include <functional>
    #ifdef _WIN32
        #include <windows.h>
    #else
        #include <dlfcn.h>
    #endif
#endif

namespace URLL
{
    // Modes as defined for the dlopen unix function, you can find documentation on them in your favourite manpage
    enum Modes
    {
        LAZY = 0x00001, // same as RTLD_LAZY
        NOW = 0x00002, // same as RTLD_NOW
        BINDING_MASK = 0x3, // same as RTLD_BINDING_MASK
        NOLOAD = 0x00004, // same as RTLD_NOLOAD
        DEEPBIND = 0x00008, // same as RTLD_DEEPBIND
        NEXT = -1l, // same as RTLD_NEXT TODO: is this right?
        DEFAULT = 0, // same as RTLD_DEFAULT TODO: is this right?
        GLOBAL = 0x00100, // same as RTLD_GLOBAL
        LOCAL = 0, // same as RTLD_LOCAL
        NODELETE = 0x01000, // same as RTLD_NODELETE
    };

#ifdef _WIN32

#else

#endif
    void* dlopen(const char* location, Modes mode) noexcept;
#ifdef URLL_USE_FUNCTIONAL
    template<typename T, typename... T2>
    void* dlsym(void* handle, const char* name, std::function<T(T2&&...)>& function) noexcept
    {
        auto* ptr = ::dlsym(handle, name);

        T(*tmp)(T2&&...);
        *(void**)(&tmp) = ptr;
        function = tmp;
        return (ptr == nullptr ? nullptr : handle);
    }
#endif
    template<typename T>
    void* dlsym(void* handle, const char* name, T& function) noexcept
    {
        auto* ptr = ::dlsym(handle, name);
        *(void**)(&function) = ptr;
        return (ptr == nullptr ? nullptr : handle);
    }

    void* dlsym(void* handle, const char* name) noexcept;

    // returns 0 on success, everything else must be an error
    int dlclose(void* handle) noexcept;

    // returns
    char* dlerror() noexcept;
}