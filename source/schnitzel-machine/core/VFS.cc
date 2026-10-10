#include "core/VFS.h"

#include <string>
#include <utility>

#include <spdlog/spdlog.h>

// Inserts or overwrites the path for `prefix`.
bool SM::VFS::mount(std::string_view prefix, std::filesystem::path path) {
    if (auto mountPoint = mountPoints.find(std::string(prefix)); mountPoint != mountPoints.end()) {
        mountPoint->second = std::move(path);
        return true;
    } else {
        auto [it, inserted] = mountPoints.try_emplace(std::string(prefix), std::move(path));
        return inserted;
    }
}

bool SM::VFS::unMount(std::string_view prefix) {
    return mountPoints.erase(prefix) != 0; 
}

bool SM::VFS::isMounted(std::string_view prefix) const {
    return mountPoints.contains(prefix);
}

bool SM::VFS::exists(std::string_view virtualPath) const {
    const auto resolved = resolve(virtualPath);
    if (!resolved)
        return false; // no mount for this prefix

    std::error_code ec; // non-throwing overload
    return std::filesystem::exists(*resolved, ec);
}

std::optional<std::filesystem::path> SM::VFS::resolve(std::string_view virtualPath) const {
    // Split "<prefix>/<rest>" at the first separator.
    // If there is no '/', find() returns npos and substr(0, npos) yields the whole string.
    const size_t slash = virtualPath.find('/');
    const auto prefix = virtualPath.substr(0, slash);
    auto rest = slash == std::string_view::npos ? std::string_view{} : virtualPath.substr(slash + 1);

    auto it = mountPoints.find(prefix);
    if (it == mountPoints.end())
        return std::nullopt;

    // Strip leading separators ("shaders//a.vert"). An absolute right-hand operand
    // would make operator/ discard the mount path entirely.
    while (!rest.empty() && rest.front() == '/') {
        rest.remove_prefix(1);
    }

    // Avoid operator/ with an empty component, which would append a trailing separator.
    if (rest.empty()) {
        return it->second;
    }
    return it->second / rest;
}
