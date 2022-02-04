#pragma once

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
    void* dlopen(const char* location) noexcept;
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
