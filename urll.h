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
            #define UVK_PUBLIC_TMPL_API __declspec(dllexport)
        #else
            #define UVK_PUBLIC_API __declspec(dllimport)
            #define UVK_PUBLIC_TMPL_API
        #endif
    #else
        #define UVK_PUBLIC_API
        #define UVK_PUBLIC_TMPL_API
    #endif
#else
    #define UVK_PUBLIC_API
    #define UVK_PUBLIC_TMPL_API
#endif

namespace URLL
{
    // UntitledImGuiFramework Event Safety - Any time
    UVK_PUBLIC_API void* dlopen(const char* location) noexcept;

    // UntitledImGuiFramework Event Safety - Any time
    UVK_PUBLIC_API void* dlsym(void* handle, const char* name) noexcept;

#ifdef URLL_USE_FUNCTIONAL
    // UntitledImGuiFramework Event Safety - Any time
    template<typename T, typename... T2>
    UVK_PUBLIC_TMPL_API void* dlsym(void* handle, const char* name, std::function<T(T2...)>& function) noexcept
    {
        void* ptr = dlsym(handle, name);

        T(*tmp)(T2...);
        *(void**)(&tmp) = ptr;
        function = tmp;
        return (ptr == nullptr ? nullptr : handle);
    }
#endif

    // UntitledImGuiFramework Event Safety - Any time
    template<typename T>
    UVK_PUBLIC_TMPL_API void* dlsym_val(void* handle, const char* name, T* var) noexcept
    {
        var = (T*)dlsym(handle, name);
        return (var == nullptr ? nullptr : handle);
    }

    // UntitledImGuiFramework Event Safety - Any time
    template<typename T>
    UVK_PUBLIC_TMPL_API void* dlsym_func(void* handle, const char* name, T& var) noexcept
    {
        *(void**)(var) = dlsym(handle, name);
        return (var == nullptr ? nullptr : handle);
    }

    // returns 0 on success, everything else must be an error
    // UntitledImGuiFramework Event Safety - Any time
    UVK_PUBLIC_API int dlclose(void* handle) noexcept;

    // returns a string with the corresponding error, if there is no error it returns null
    // UntitledImGuiFramework Event Safety - Any time
    UVK_PUBLIC_API char* dlerror() noexcept;
}
