#include "urll.h"
#include <string>

#ifdef _WIN32
namespace
{
    // On Unix ::dlerror() only reports failures raised by the loader API itself and clears the error
    // when it's read. GetLastError() is process-wide state that any unrelated Win32 call can
    // overwrite, so instead of reading it directly in dlerror() we record it here, at the point where
    // an URLL operation actually fails. Both are thread_local, matching glibc's thread-local
    // ::dlerror() and making the buffer returned by dlerror() free of data races.
    thread_local DWORD urllLastError = 0;
    thread_local std::string urllErrorString;

    // Win32 functions only guarantee a meaningful last-error value after a failure, hence the
    // fallback for the rare case where a function fails without setting one
    void recordLastError(const DWORD fallback) noexcept
    {
        const DWORD id = GetLastError();
        urllLastError = (id == 0 ? fallback : id);
    }
}
#endif

void* URLL::dlopen(const char* location) noexcept
{
    // Due to restrictions from Windows libraries are always loaded in lazy mode!
#ifdef _WIN32
    // Emulates the Unix behaviour of dlopen(nullptr), which returns a handle to the main program.
    // Unlike LoadLibrary, GetModuleHandle doesn't increment the module's reference count, which is
    // why dlclose has to skip it
    if (location == nullptr)
    {
        void* handle = (void*)GetModuleHandleA(nullptr);
        if (handle == nullptr)
            recordLastError(ERROR_MOD_NOT_FOUND);
        return handle;
    }

    // LoadLibraryA instead of LoadLibrary, since the TCHAR-generic macro resolves to the wide
    // version in UNICODE builds, which doesn't accept our char* argument
    SetLastError(0);
    void* handle = (void*)LoadLibraryA(location);
    if (handle == nullptr)
        recordLastError(ERROR_MOD_NOT_FOUND);
    return handle;
#else
    return ::dlopen(location, RTLD_LAZY);
#endif
}

void* URLL::dlsym(void* handle, const char* name) noexcept
{
#ifdef _WIN32
    SetLastError(0);
    // The cast is required because GetProcAddress returns a function pointer, which doesn't
    // implicitly convert to void* outside of Microsoft's extensions
    void* ret = (void*)GetProcAddress((HMODULE)handle, name);
    if (ret == nullptr)
        recordLastError(ERROR_PROC_NOT_FOUND);
    return ret;
#else
    return ::dlsym(handle, name);
#endif
}

int URLL::dlclose(void* handle) noexcept
{
#ifdef _WIN32
    // The main program handle handed out by dlopen(nullptr) was never reference counted, so
    // releasing it would decrement a count we never incremented. A caller that instead loaded the
    // executable by path gets that same handle back with its count incremented, and leaks the one
    // reference here, which costs nothing given the main image is never unloaded anyway
    if (handle == (void*)GetModuleHandleA(nullptr))
        return 0;

    // Needed because Unix and Microsoft software go the oposite way when talking about errors
    SetLastError(0);
    if (FreeLibrary((HMODULE)handle) == 0)
    {
        recordLastError(ERROR_INVALID_HANDLE);
        return -1;
    }
    return 0;
#else
    return ::dlclose(handle);
#endif
}

char* URLL::dlerror() noexcept
{
#ifdef _WIN32
    const DWORD id = urllLastError;
    if (id == 0)
        return nullptr;
    // Unix clears the error once it has been read
    urllLastError = 0;

    char* buf = nullptr;
    // FormatMessageA instead of FormatMessage, since the TCHAR-generic macro resolves to the wide
    // version in UNICODE builds and would fill our char* buffer with UTF-16 data. The language is
    // left at 0 so the system walks its own fallback order, instead of failing outright whenever
    // the message table happens to lack one specific language
    const DWORD len = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, id, 0, reinterpret_cast<LPSTR>(&buf), 0, nullptr);

    // Everything below allocates, and we're noexcept, so a failure to build the message can only
    // be reported as the absence of one
    try
    {
        // The string outlives the call that filled it, so a message we fail to build now must not
        // leave the previous one in place to be reported a second time
        urllErrorString.clear();
        if (len != 0 && buf != nullptr)
        {
            urllErrorString.assign(buf, len);
            // System messages come terminated with a CRLF, which would break the line of every log
            // message that embeds the error
            const size_t end = urllErrorString.find_last_not_of("\r\n");
            urllErrorString.erase(end == std::string::npos ? 0 : end + 1);
        }

        // Losing the error entirely because the system has no message for it is worse than an
        // ugly message
        if (urllErrorString.empty())
            urllErrorString = "Unknown error " + std::to_string(static_cast<unsigned long>(id));
    }
    catch (...)
    {
        // The error was consumed above, so leaving it cleared would make the next call report that
        // nothing ever went wrong
        urllLastError = id;
        if (buf != nullptr)
            LocalFree(buf);
        return nullptr;
    }

    if (buf != nullptr)
        LocalFree(buf);
    return urllErrorString.data();
#else
    return ::dlerror();
#endif
}
