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

#ifdef MLS_EXPORT_LIBRARY
    #ifdef _WIN32
        #ifdef MLS_LIB_COMPILE
            #define MLS_PUBLIC_API __declspec(dllexport)
        #else
            #define MLS_PUBLIC_API __declspec(dllimport)
        #endif
    #else
        #define MLS_PUBLIC_API
    #endif
#else
    #define MLS_PUBLIC_API
#endif

namespace URLL
{
    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API void* dlopen(const char* location) noexcept;

    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API void* dlsym(void* handle, const char* name) noexcept;

#ifdef URLL_USE_FUNCTIONAL
    // UntitledImGuiFramework Event Safety - Any time
    template<typename T, typename... T2>
    void* dlsym(void* handle, const char* name, std::function<T(T2...)>& function) noexcept
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
    void* dlsym_val(void* handle, const char* name, T* var) noexcept
    {
        var = (T*)dlsym(handle, name);
        return (var == nullptr ? nullptr : handle);
    }

    // UntitledImGuiFramework Event Safety - Any time
    template<typename T>
    void* dlsym_func(void* handle, const char* name, T& var) noexcept
    {
        *(void**)(var) = dlsym(handle, name);
        return (var == nullptr ? nullptr : handle);
    }

    // returns 0 on success, everything else must be an error
    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API int dlclose(void* handle) noexcept;

    // returns a string with the corresponding error, if there is no error it returns null
    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API char* dlerror() noexcept;
}
