#ifndef SM_RHI_VULKANCONFIG_H_
#define SM_RHI_VULKANCONFIG_H_

#include <algorithm>
#include <array>
#include <cstring>
#include <vector>

#include <vulkan/vulkan.h>

namespace SM {
    inline bool hasExtension(const std::vector<VkExtensionProperties> &extensions, const std::string_view &name)
    {
        const auto it = std::find_if(extensions.begin(),
                                     extensions.end(),
                                     [name](const VkExtensionProperties &ext) { return ext.extensionName == name; });
        return it != extensions.end();
    };

    inline bool hasExtension(const std::vector<const char *> &extensions, const char *targetExtension) 
    {
        const auto it = std::find_if(extensions.begin(),
                                     extensions.end(),
                                     [&](const char *ext) { return strcmp(ext, targetExtension) == 0; });
        return it != extensions.end();
    }

    
    // RELEASE    
#if defined(SM_BUILD_RELEASE_MODE) || defined(__arm__)
    const std::vector<const char *> requestedInstanceLayers = {
#ifdef __APPLE__
        "VK_LAYER_KHRONOS_synchronization2"
#endif
    };
    
    // DEBUG  
#else
    const std::vector<const char *> requestedInstanceLayers = {
        "VK_LAYER_KHRONOS_validation",
#ifdef __APPLE__
        "VK_LAYER_KHRONOS_synchronization2"
#endif
    };
#endif

    constexpr std::array<const char *, 1> requestedInstanceExtensions = {
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };

    inline std::vector<const char *> getDefaultRequestedInstanceExtensions()
    {
        std::vector<const char *> extensions{
            VK_KHR_SURFACE_EXTENSION_NAME,
#if VK_KHR_xcb_surface
            VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#endif
#if VK_KHR_wayland_surface
            VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
#endif
#if VK_KHR_win32_surface
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
#if VK_KHR_surface_maintenance1
            VK_KHR_SURFACE_MAINTENANCE_1_EXTENSION_NAME,
#endif
#if VK_KHR_get_surface_capabilities2
            VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
#endif
#if defined(KDGPU_PLATFORM_APPLE)
            VK_EXT_METAL_SURFACE_EXTENSION_NAME,
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
            VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
#elif defined(KDGPU_PLATFORM_ANDROID)
            VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
#endif
        };

#if VK_EXT_debug_utils
#ifdef SM_BUILD_DEBUG_MODE
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
#endif
        return extensions;
    }


    // Device config assuming Vulkan 1.2
    inline std::vector<const char *> getDefaultRequestedDeviceExtensions()
    {
        std::vector<const char *> extensions{
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#if VK_KHR_swapchain_maintenance1
            VK_KHR_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME,
#endif
#if VK_EXT_host_image_copy
            VK_KHR_COPY_COMMANDS_2_EXTENSION_NAME, // Needed by VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME
            VK_KHR_FORMAT_FEATURE_FLAGS_2_EXTENSION_NAME, // Needed by VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME
            VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME,
#endif
#if VK_KHR_external_semaphore_fd
            VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME,
#endif
#if VK_KHR_external_semaphore_win32
            VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME,
#endif
#if VK_KHR_external_fence_fd
            VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME,
#endif
#if VK_KHR_external_fence_win32
            VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME,
#endif
#if VK_KHR_external_memory_fd
            VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME,
#endif
#if VK_KHR_external_memory_win32
            VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME,
#endif
#if VK_EXT_external_memory_dma_buf
            VK_EXT_EXTERNAL_MEMORY_DMA_BUF_EXTENSION_NAME,
#endif
#if VK_KHR_deferred_host_operations
            VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
#endif
#if VK_KHR_ray_tracing_pipeline
            VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
#endif
#if VK_KHR_acceleration_structure
            VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
#endif
#if VK_KHR_fragment_shading_rate
            VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME,
#endif
#if VK_EXT_mesh_shader
            VK_EXT_MESH_SHADER_EXTENSION_NAME,
#endif
#if VK_EXT_image_drm_format_modifier
            VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME,
#endif
#if VK_KHR_synchronization2
            VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
#endif
#if defined(KDGPU_PLATFORM_MACOS)
            VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME,
#endif
#if VK_KHR_shader_non_semantic_info
            VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME,
#endif
#if VK_KHR_sampler_ycbcr_conversion
            VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME,
#endif
#if VK_KHR_push_descriptor
            VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME,
#endif
#if VK_KHR_dynamic_rendering
            VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
#endif
#if VK_KHR_dynamic_rendering_local_read
            VK_KHR_DYNAMIC_RENDERING_LOCAL_READ_EXTENSION_NAME,
#endif

            // Extensions needed for Vulkan 1.1 features that are core in 1.2
#if VK_EXT_descriptor_indexing
            VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
#endif
#if VK_KHR_create_renderpass2
            VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
#endif
#if VK_KHR_uniform_buffer_standard_layout
            VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_EXTENSION_NAME,
#endif
#if VK_KHR_buffer_device_address
            VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
#endif
#if VK_KHR_timeline_semaphore
            VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME,
#endif
        };

        return extensions;
    }

} // namespace SM

#endif // SM_RHI_VULKANCONFIG_H_
