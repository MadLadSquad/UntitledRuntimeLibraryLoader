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
    // Libraries are always loaded in lazy mode. A null location returns a handle to the main
    // program, on Windows too, but there it only resolves symbols exported by the executable
    // itself, not by the libraries it links against
    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API void* urll_dlopen(const char* location);

    // A null handle is not supported. On glibc it happens to search every loaded library, since
    // RTLD_DEFAULT is null there, but Windows offers no cheap equivalent and simply fails
    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API void* urll_dlsym(void* handle, const char* name);
    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API void* urll_dlsym_func(void* handle, const char* name, void** function);
    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API void* urll_dlsym_var(void* handle, const char* name, void** var);

    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API int urll_dlclose(void* handle);

    // returns a string with the corresponding error, if there is no error it returns null. Reading
    // the error clears it, so 2 calls in a row will always report null the second time. The buffer
    // is owned by the calling thread and is invalidated by that thread's next urll_dlerror() call
    // UntitledImGuiFramework Event Safety - Any time
    MLS_PUBLIC_API char* urll_dlerror();
#ifdef __cplusplus
};
#endif