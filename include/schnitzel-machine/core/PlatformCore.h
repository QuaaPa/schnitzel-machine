#include <filesystem>
#include <string>
#include <vector>
#include "RHI/Device.h"

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

namespace SM {
    // Crossplatform exedir querying 
    SM_NODISCARD std::filesystem::path getExecutablePath() {        
#if defined(_WIN32)
        std::wstring buf(MAX_PATH, L'\0');
        for (;;) {
            DWORD n = GetModuleFileNameW(nullptr, buf.data(), static_cast<DWORD>(buf.size()));
            if (n == 0) return {};
            if (n < buf.size()) {
                buf.resize(n);
                return buf;
            }
            buf.resize(buf.size() * 2); 
        }
#elif defined(__APPLE__)
        uint32_t size = 0;
        _NSGetExecutablePath(nullptr, &size);
        std::string buf(size, '\0');
        if (_NSGetExecutablePath(buf.data(), &size) != 0) return {};
        return std::filesystem::weakly_canonical(buf.c_str());
#else // Linux
        std::error_code ec;
        auto p = std::filesystem::read_symlink("/proc/self/exe", ec);
        return ec ? std::filesystem::path{} : p;
#endif
    }
} // namespace SM
