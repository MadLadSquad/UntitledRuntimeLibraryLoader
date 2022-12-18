#include "urll.h"

void* URLL::dlopen(const char* location) noexcept
{
    // Due to restrictions from Windows libraries are always loaded in lazy mode!
#ifdef _WIN32
    return (void*)LoadLibrary(location);
#else
    return ::dlopen(location, RTLD_LAZY);
#endif
}

void* URLL::dlsym(void* handle, const char* name) noexcept
{
#ifdef _WIN32
    return GetProcAddress((HINSTANCE)handle, name);
#else
    return ::dlsym(handle, name);
#endif
}

int URLL::dlclose(void* handle) noexcept
{
#ifdef _WIN32
    // Needed because Unix and Microsoft software go the oposite way when talking about errors
    return (FreeLibrary((HINSTANCE)handle) == 0 ? -1 : 0);
#else
    return ::dlclose(handle);
#endif
}

char* URLL::dlerror() noexcept
{
#ifdef _WIN32
    // We use this in order to have it return an accurate value
    static std::string str;

    uint64_t ID = GetLastError();
    if (ID == 0)
        return nullptr;
    char* buf = nullptr;
    size_t len = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, ID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPTSTR>(&buf), 0, nullptr);
    if (!len)
        return nullptr;
    str = std::string(buf, len);
    
    LocalFree(buf);
    return str.data();
#else
    return ::dlerror();
#endif
}