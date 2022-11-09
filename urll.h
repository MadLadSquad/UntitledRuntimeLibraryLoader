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

#ifdef UVK_LOG_EXPORT_FROM_LIBRARY
    #ifdef _WIN32
        #ifdef UVK_LIB_COMPILE
            #define UVK_PUBLIC_API __declspec(dllexport)
        #else
            #define UVK_PUBLIC_API __declspec(dllimport)
        #endif
    #else
        #define UVK_PUBLIC_API
    #endif
#else
    #define UVK_PUBLIC_API
#endif

namespace URLL
{
    UVK_PUBLIC_API void* dlopen(const char* location) noexcept;
#ifdef URLL_USE_FUNCTIONAL
    template<typename T, typename... T2>
    void* dlsym(void* handle, const char* name, std::function<T(T2...)>& function) noexcept
    {
#ifdef _WIN32
        auto* ptr = GetProcAddress((HINSTANCE)handle, name);
#else
        auto* ptr = ::dlsym(handle, name);
#endif
        T(*tmp)(T2...);
        *(void**)(&tmp) = ptr;
        function = tmp;
        return (ptr == nullptr ? nullptr : handle);
    }
#endif

    template<typename T>
    void* dlsym_val(void* handle, const char* name, T* var) noexcept
    {
#ifdef _WIN32
        var = (T*)GetProcAddress((HINSTANCE)handle, name);
#else
        var = (T*)::dlsym(handle, name);
#endif
        return (var == nullptr ? nullptr : handle);
    }


    template<typename T>
    void* dlsym_func(void* handle, const char* name, T& var) noexcept
    {
#ifdef _WIN32
        *(void**)(&var) = GetProcAddress((HINSTANCE)handle, name);
#else
        *(void**)(&var) = ::dlsym(handle, name);
#endif
        return (var == nullptr ? nullptr : handle);
    }

    UVK_PUBLIC_API void* dlsym(void* handle, const char* name) noexcept;

    // returns 0 on success, everything else must be an error
    UVK_PUBLIC_API int dlclose(void* handle) noexcept;

    // returns a string with the corresponding error, if there is no error it returns null
    UVK_PUBLIC_API char* dlerror() noexcept;
}
