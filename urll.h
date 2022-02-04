#pragma once
#define URLL_USE_FUNCTIONAL

// Define this if you want to use std::function for easier function handling!
#ifdef URLL_USE_FUNCTIONAL
    #include <functional>
#endif
#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
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

    void* dlopen(const char* location, Modes mode) noexcept;
#ifdef URLL_USE_FUNCTIONAL
    template<typename T, typename... T2>
    void* dlsym(void* handle, const char* name, std::function<T(T2&&...)>& function) noexcept
    {
#ifdef _WIN32
        auto* ptr = GetProcAddress((HINSTANCE)handle, name);

        T(*tmp)(T2&&...);
        *(void**)(&tmp) = ptr;
        function = tmp;
        return (ptr == nullptr ? nullptr : handle);
#else
        auto* ptr = ::dlsym(handle, name);

        T(*tmp)(T2&&...);
        *(void**)(&tmp) = ptr;
        function = tmp;
        return (ptr == nullptr ? nullptr : handle);
#endif
    }
#endif
    template<typename T>
    void* dlsym(void* handle, const char* name, T& function) noexcept
    {
#ifdef _WIN32
        auto* ptr = GetProcAddress((HINSTANCE)handle, name);
        *(void**)(&function) = ptr;
        return (ptr == nullptr ? nullptr : handle)
#else
        auto* ptr = ::dlsym(handle, name);
        *(void**)(&function) = ptr;
        return (ptr == nullptr ? nullptr : handle);
#endif
    }

    void* dlsym(void* handle, const char* name) noexcept;

    // returns 0 on success, everything else must be an error
    int dlclose(void* handle) noexcept;

    // returns a string with the corresponding error, if there is no error it returns null
    char* dlerror() noexcept;
}