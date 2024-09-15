#pragma once

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

#ifdef __cplusplus
extern "C"
{
#endif
    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API void* urll_dlopen(const char* location);

    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API void* urll_dlsym(void* handle, const char* name);
    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API void* urll_dlsym_func(void* handle, const char* name, void** function);
    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API void* urll_dlsym_var(void* handle, const char* name, void* var);

    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API int urll_dlclose(void* handle);

    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API char* urll_dlerror();
#ifdef __cplusplus
};
#endif