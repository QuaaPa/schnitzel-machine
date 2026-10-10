#ifndef SM_CORE_VFS_H_
#define SM_CORE_VFS_H_

#include <filesystem>
#include <map>
#include <optional>
#include <string_view>

#include "Macros.h"

namespace SM {
    class VFS {
    public:
        bool mount(std::string_view prefix, std::filesystem::path path);
        bool unMount(std::string_view prefix);
        bool isMounted(std::string_view prefix) const;
        bool exists(std::string_view virtualPath) const;
                
        SM_NODISCARD std::optional<std::filesystem::path> resolve(std::string_view virtualPath) const;
        
    private:
        std::map<std::string, std::filesystem::path, std::less<>> mountPoints;
    };

} // namespace SM

#endif // SM_CORE_VFS_H_
