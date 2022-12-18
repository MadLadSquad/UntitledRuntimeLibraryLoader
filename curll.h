#pragma once

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

#ifdef __cplusplus
extern "C"
{
#endif
    UVK_PUBLIC_API void* urll_dlopen(const char* location);

    UVK_PUBLIC_API void* urll_dlsym(void* handle, const char* name);
    UVK_PUBLIC_API void* urll_dlsym_func(void* handle, const char* name, void** function);
    UVK_PUBLIC_API void* urll_dlsym_var(void* handle, const char* name, void* var);

    UVK_PUBLIC_API int urll_dlclose(void* handle);

    UVK_PUBLIC_API char* urll_dlerror();
#ifdef __cplusplus
};
#endif