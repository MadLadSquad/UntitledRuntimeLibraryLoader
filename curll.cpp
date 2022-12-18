#include "curll.h"
#include "urll.h"

#ifdef __cplusplus
extern "C"
{
#endif
    void* urll_dlopen(const char* location)
    {
        return URLL::dlopen(location);
    }

    void* urll_dlsym(void* handle, const char* name)
    {
        return URLL::dlsym(handle, name);
    }

    void* urll_dlsym_func(void* handle, const char* name, void** function)
    {
        *function = URLL::dlsym(handle, name);
        return (*function == nullptr ? nullptr : handle);
    }

    void* urll_dlsym_var(void* handle, const char* name, void* var)
    {
         var = URLL::dlsym(handle, name);
         return (var == nullptr ? nullptr : handle);
    }

    int urll_dlclose(void* handle)
    {
        return URLL::dlclose(handle);
    }

    char* urll_dlerror()
    {
        return URLL::dlerror();
    }
#ifdef __cplusplus
}
#endif