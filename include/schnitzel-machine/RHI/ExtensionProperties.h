#ifndef SM_RHI_EXTENSIONPROPERTIES_H_
#define SM_RHI_EXTENSIONPROPERTIES_H_

#include <algorithm>
#include <cstring>
#include <string_view>
#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace SM {    
    struct ExtensionProperties {
        char extensionName[VK_MAX_EXTENSION_NAME_SIZE];
        uint32_t specVersion;
    };

    inline bool hasExtension(const std::vector<SM::ExtensionProperties> &extensions, const std::string_view &name)
    {
        const auto it = std::find_if(extensions.begin(),
                                     extensions.end(),
                                     [name](const SM::ExtensionProperties &ext) { return ext.extensionName == name; });
        return it != extensions.end();
    };

    inline bool hasExtension(const std::vector<const char *> &extensions, const char *targetExtension) 
    {
        const auto it = std::find_if(extensions.begin(),
                                     extensions.end(),
                                     [&](const char *ext) { return strcmp(ext, targetExtension) == 0; });
        return it != extensions.end();
    }


}; // namespae SM

#endif // SM_RHI_EXTENSIONPROPERTIES_H_
