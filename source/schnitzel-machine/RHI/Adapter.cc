#include "RHI/Adapter.h"

#include <vulkan/vulkan_core.h>

#include "core/Log.h"
#include "RHI/AdapterFeatures.h"
#include "RHI/AdapterSwapchainProperties.h"

std::vector<VkExtensionProperties> SM::Adapter::extensions() const
{
    if(m_handle == VK_NULL_HANDLE) {
        SM_LOG_ERROR("RHI/Adapter", "Failed to query adapter's extensions, invalid adapter");
        return {};
    }

    uint32_t extensionCount = 0;
    if (vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extensionCount, nullptr) != VK_SUCCESS) {
        SM_LOG_ERROR("RHI/Adapter", "Unable to enumerate instance extensions");
        return {};
    }

    std::vector<VkExtensionProperties> vkExtensions(extensionCount);
    if (vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extensionCount, vkExtensions.data()) != VK_SUCCESS) {
        SM_LOG_ERROR("RHI/Adapter", "Unable to query instance extensions");
        return {};
    }    

    return vkExtensions;
}

VkPhysicalDeviceProperties SM::Adapter::properties() const{
    if(m_handle == VK_NULL_HANDLE) {
        SM_LOG_ERROR("RHI/Adapter", "Failed to query adapter's properties, invalid adapter");
        return {};
    }
    VkPhysicalDeviceProperties vkProperties;
    vkGetPhysicalDeviceProperties(m_handle, &vkProperties);
    return vkProperties;
}

// VkPhysicalDeviceFeatures SM::Adapter::features() const{
//     if(m_handle == VK_NULL_HANDLE) {
//         SM_LOG_ERROR("RHI/Adapter", "Failed to query adapter's features, invalid adapter");
//         return {};
//     }
//     VkPhysicalDeviceFeatures vkFeatures;
//     vkGetPhysicalDeviceFeatures(m_handle, &vkFeatures);
//     return vkFeatures;
// }

SM::AdapterFeatures SM::Adapter::features() {
   VkBaseOutStructure *chainCurrent{ nullptr };
   auto addToChain = [&chainCurrent](auto *next) {
       auto n = reinterpret_cast<VkBaseOutStructure *>(next);
       chainCurrent->pNext = n;
       chainCurrent = n;
   };

   VkPhysicalDeviceFeatures2 deviceFeatures2{};
   deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
   chainCurrent = reinterpret_cast<VkBaseOutStructure *>(&deviceFeatures2);

   VkPhysicalDeviceMultiviewFeatures multiViewFeatures{};
   multiViewFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES;
   addToChain(&multiViewFeatures);

   VkPhysicalDeviceUniformBufferStandardLayoutFeatures stdLayoutFeatures{};
   stdLayoutFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES;
   addToChain(&stdLayoutFeatures);

   VkPhysicalDeviceDescriptorIndexingFeatures deviceDescriptorIndexingFeatures{};
   deviceDescriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
   addToChain(&deviceDescriptorIndexingFeatures);

   VkPhysicalDeviceVulkan12Features physicalDeviceFeatures12{};
   physicalDeviceFeatures12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
   addToChain(&physicalDeviceFeatures12);

#if VK_KHR_fragment_shading_rate
   VkPhysicalDeviceFragmentShadingRateFeaturesKHR fragmentShadingRateFeatures{};
   fragmentShadingRateFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR;
   addToChain(&fragmentShadingRateFeatures);
#endif

#if VK_KHR_acceleration_structure
   VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeaturesKhr{};
   accelerationStructureFeaturesKhr.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
   addToChain(&accelerationStructureFeaturesKhr);
#endif

#if VK_KHR_ray_tracing_pipeline
   VkPhysicalDeviceRayTracingPipelineFeaturesKHR rayTracingPipelineFeaturesKhr{};
   rayTracingPipelineFeaturesKhr.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
   addToChain(&rayTracingPipelineFeaturesKhr);
#endif

#if VK_KHR_synchronization2
   VkPhysicalDeviceSynchronization2Features synchronization2Features{};
   synchronization2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
   addToChain(&synchronization2Features);
#endif

#if VK_EXT_mesh_shader
   VkPhysicalDeviceMeshShaderFeaturesEXT meshShaderFeatures{};
   meshShaderFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;
   addToChain(&meshShaderFeatures);
#endif

#if VK_EXT_host_image_copy
   VkPhysicalDeviceHostImageCopyFeaturesEXT hostImageCopyFeatures{};
   hostImageCopyFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_FEATURES_EXT;
   addToChain(&hostImageCopyFeatures);
#endif

#if VK_KHR_sampler_ycbcr_conversion
   VkPhysicalDeviceSamplerYcbcrConversionFeaturesKHR ycbcrConversionFeatures{};
   ycbcrConversionFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES_KHR;
   addToChain(&ycbcrConversionFeatures);
#endif

#if VK_KHR_dynamic_rendering
   VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeatures{};
   dynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
   addToChain(&dynamicRenderingFeatures);
#endif

#if VK_KHR_dynamic_rendering_local_read
   VkPhysicalDeviceDynamicRenderingLocalReadFeaturesKHR dynamicLocalReadFeatures{};
   dynamicLocalReadFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_LOCAL_READ_FEATURES_KHR;
   addToChain(&dynamicLocalReadFeatures);
#endif

#if VK_KHR_swapchain_maintenance1
   VkPhysicalDeviceSwapchainMaintenance1FeaturesKHR swapchainMaintenanceFeatures{};
   swapchainMaintenanceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SWAPCHAIN_MAINTENANCE_1_FEATURES_KHR;
   addToChain(&swapchainMaintenanceFeatures);
#endif
#if VK_KHR_timeline_semaphore
   VkPhysicalDeviceTimelineSemaphoreFeaturesKHR timelineSemaphoreFeatures{};
   timelineSemaphoreFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR;
   addToChain(&timelineSemaphoreFeatures);
#endif

   vkGetPhysicalDeviceFeatures2(m_handle, &deviceFeatures2);
   const VkPhysicalDeviceFeatures &deviceFeatures = deviceFeatures2.features;

   SM::AdapterFeatures features = {
       .robustBufferAccess = static_cast<bool>(deviceFeatures.robustBufferAccess),
       .fullDrawIndexUint32 = static_cast<bool>(deviceFeatures.fullDrawIndexUint32),
       .imageCubeArray = static_cast<bool>(deviceFeatures.imageCubeArray),
       .independentBlend = static_cast<bool>(deviceFeatures.independentBlend),
       .geometryShader = static_cast<bool>(deviceFeatures.geometryShader),
       .tessellationShader = static_cast<bool>(deviceFeatures.tessellationShader),
       .sampleRateShading = static_cast<bool>(deviceFeatures.sampleRateShading),
       .dualSrcBlend = static_cast<bool>(deviceFeatures.dualSrcBlend),
       .logicOp = static_cast<bool>(deviceFeatures.logicOp),
       .multiDrawIndirect = static_cast<bool>(deviceFeatures.multiDrawIndirect),
       .drawIndirectFirstInstance = static_cast<bool>(deviceFeatures.drawIndirectFirstInstance),
       .depthClamp = static_cast<bool>(deviceFeatures.depthClamp),
       .depthBiasClamp = static_cast<bool>(deviceFeatures.depthBiasClamp),
       .fillModeNonSolid = static_cast<bool>(deviceFeatures.fillModeNonSolid),
       .depthBounds = static_cast<bool>(deviceFeatures.depthBounds),
       .wideLines = static_cast<bool>(deviceFeatures.wideLines),
       .largePoints = static_cast<bool>(deviceFeatures.largePoints),
       .alphaToOne = static_cast<bool>(deviceFeatures.alphaToOne),
       .multiViewport = static_cast<bool>(deviceFeatures.multiViewport),
       .samplerAnisotropy = static_cast<bool>(deviceFeatures.samplerAnisotropy),
       .textureCompressionETC2 = static_cast<bool>(deviceFeatures.textureCompressionETC2),
       .textureCompressionASTC_LDR = static_cast<bool>(deviceFeatures.textureCompressionASTC_LDR),
       .textureCompressionBC = static_cast<bool>(deviceFeatures.textureCompressionBC),
       .occlusionQueryPrecise = static_cast<bool>(deviceFeatures.occlusionQueryPrecise),
       .pipelineStatisticsQuery = static_cast<bool>(deviceFeatures.pipelineStatisticsQuery),
       .vertexPipelineStoresAndAtomics = static_cast<bool>(deviceFeatures.vertexPipelineStoresAndAtomics),
       .fragmentStoresAndAtomics = static_cast<bool>(deviceFeatures.fragmentStoresAndAtomics),
       .shaderTessellationAndGeometryPointSize = static_cast<bool>(deviceFeatures.shaderTessellationAndGeometryPointSize),
       .shaderImageGatherExtended = static_cast<bool>(deviceFeatures.shaderImageGatherExtended),
       .shaderStorageImageExtendedFormats = static_cast<bool>(deviceFeatures.shaderStorageImageExtendedFormats),
       .shaderStorageImageMultisample = static_cast<bool>(deviceFeatures.shaderStorageImageMultisample),
       .shaderStorageImageReadWithoutFormat = static_cast<bool>(deviceFeatures.shaderStorageImageReadWithoutFormat),
       .shaderStorageImageWriteWithoutFormat = static_cast<bool>(deviceFeatures.shaderStorageImageWriteWithoutFormat),
       .shaderUniformBufferArrayDynamicIndexing = static_cast<bool>(deviceFeatures.shaderUniformBufferArrayDynamicIndexing),
       .shaderSampledImageArrayDynamicIndexing = static_cast<bool>(deviceFeatures.shaderSampledImageArrayDynamicIndexing),
       .shaderStorageBufferArrayDynamicIndexing = static_cast<bool>(deviceFeatures.shaderStorageBufferArrayDynamicIndexing),
       .shaderStorageImageArrayDynamicIndexing = static_cast<bool>(deviceFeatures.shaderStorageImageArrayDynamicIndexing),
       .shaderClipDistance = static_cast<bool>(deviceFeatures.shaderClipDistance),
       .shaderCullDistance = static_cast<bool>(deviceFeatures.shaderCullDistance),
       .shaderFloat64 = static_cast<bool>(deviceFeatures.shaderFloat64),
       .shaderInt64 = static_cast<bool>(deviceFeatures.shaderInt64),
       .shaderInt16 = static_cast<bool>(deviceFeatures.shaderInt16),
       .shaderResourceResidency = static_cast<bool>(deviceFeatures.shaderResourceResidency),
       .shaderResourceMinLod = static_cast<bool>(deviceFeatures.shaderResourceMinLod),
       .sparseBinding = static_cast<bool>(deviceFeatures.sparseBinding),
       .sparseResidencyBuffer = static_cast<bool>(deviceFeatures.sparseResidencyBuffer),
       .sparseResidencyImage2D = static_cast<bool>(deviceFeatures.sparseResidencyImage2D),
       .sparseResidencyImage3D = static_cast<bool>(deviceFeatures.sparseResidencyImage3D),
       .sparseResidency2Samples = static_cast<bool>(deviceFeatures.sparseResidency2Samples),
       .sparseResidency4Samples = static_cast<bool>(deviceFeatures.sparseResidency4Samples),
       .sparseResidency8Samples = static_cast<bool>(deviceFeatures.sparseResidency8Samples),
       .sparseResidency16Samples = static_cast<bool>(deviceFeatures.sparseResidency16Samples),
       .sparseResidencyAliased = static_cast<bool>(deviceFeatures.sparseResidencyAliased),
       .variableMultisampleRate = static_cast<bool>(deviceFeatures.variableMultisampleRate),
       .inheritedQueries = static_cast<bool>(deviceFeatures.inheritedQueries),
       .uniformBufferStandardLayout = static_cast<bool>(stdLayoutFeatures.uniformBufferStandardLayout),
       .multiView = static_cast<bool>(multiViewFeatures.multiview),
       .multiViewGeometryShader = static_cast<bool>(multiViewFeatures.multiviewGeometryShader),
       .multiViewTessellationShader = static_cast<bool>(multiViewFeatures.multiviewTessellationShader),
       .shaderInputAttachmentArrayDynamicIndexing = static_cast<bool>(deviceDescriptorIndexingFeatures.shaderInputAttachmentArrayDynamicIndexing),
       .shaderUniformTexelBufferArrayDynamicIndexing = static_cast<bool>(deviceDescriptorIndexingFeatures.shaderUniformTexelBufferArrayDynamicIndexing),
       .shaderStorageTexelBufferArrayDynamicIndexing = static_cast<bool>(deviceDescriptorIndexingFeatures.shaderStorageTexelBufferArrayDynamicIndexing),
       .shaderUniformBufferArrayNonUniformIndexing = static_cast<bool>(deviceDescriptorIndexingFeatures.shaderUniformBufferArrayNonUniformIndexing),
       .shaderSampledImageArrayNonUniformIndexing = static_cast<bool>(deviceDescriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing),
       .shaderStorageBufferArrayNonUniformIndexing = static_cast<bool>(deviceDescriptorIndexingFeatures.shaderStorageBufferArrayNonUniformIndexing),
       .shaderStorageImageArrayNonUniformIndexing = static_cast<bool>(deviceDescriptorIndexingFeatures.shaderStorageImageArrayNonUniformIndexing),
       .shaderInputAttachmentArrayNonUniformIndexing = static_cast<bool>(deviceDescriptorIndexingFeatures.shaderInputAttachmentArrayNonUniformIndexing),
       .shaderUniformTexelBufferArrayNonUniformIndexing = static_cast<bool>(deviceDescriptorIndexingFeatures.shaderUniformTexelBufferArrayNonUniformIndexing),
       .shaderStorageTexelBufferArrayNonUniformIndexing = static_cast<bool>(deviceDescriptorIndexingFeatures.shaderStorageTexelBufferArrayNonUniformIndexing),
       .bindGroupBindingUniformBufferUpdateAfterBind = static_cast<bool>(deviceDescriptorIndexingFeatures.descriptorBindingUniformBufferUpdateAfterBind),
       .bindGroupBindingSampledImageUpdateAfterBind = static_cast<bool>(deviceDescriptorIndexingFeatures.descriptorBindingSampledImageUpdateAfterBind),
       .bindGroupBindingStorageImageUpdateAfterBind = static_cast<bool>(deviceDescriptorIndexingFeatures.descriptorBindingStorageImageUpdateAfterBind),
       .bindGroupBindingStorageBufferUpdateAfterBind = static_cast<bool>(deviceDescriptorIndexingFeatures.descriptorBindingStorageBufferUpdateAfterBind),
       .bindGroupBindingUniformTexelBufferUpdateAfterBind = static_cast<bool>(deviceDescriptorIndexingFeatures.descriptorBindingUniformTexelBufferUpdateAfterBind),
       .bindGroupBindingStorageTexelBufferUpdateAfterBind = static_cast<bool>(deviceDescriptorIndexingFeatures.descriptorBindingStorageTexelBufferUpdateAfterBind),
       .bindGroupBindingUpdateUnusedWhilePending = static_cast<bool>(deviceDescriptorIndexingFeatures.descriptorBindingUpdateUnusedWhilePending),
       .bindGroupBindingPartiallyBound = static_cast<bool>(deviceDescriptorIndexingFeatures.descriptorBindingPartiallyBound),
       .bindGroupBindingVariableDescriptorCount = static_cast<bool>(deviceDescriptorIndexingFeatures.descriptorBindingVariableDescriptorCount),
       .runtimeBindGroupArray = static_cast<bool>(deviceDescriptorIndexingFeatures.runtimeDescriptorArray),
       .bufferDeviceAddress = static_cast<bool>(physicalDeviceFeatures12.bufferDeviceAddress),
       .pipelineFragmentShadingRate = false,
       .primitiveFragmentShadingRate = false,
       .attachmentFragmentShadingRate = false,
       .accelerationStructures = false,
       .rayTracingPipeline = false,
       .rayTracingPipelineShaderGroupHandleCaptureReplay = false,
       .rayTracingPipelineShaderGroupHandleCaptureReplayMixed = false,
       .rayTracingPipelineTraceRaysIndirect = false,
       .rayTraversalPrimitiveCulling = false,
       .taskShader = false,
       .meshShader = false,
       .multiviewMeshShader = false,
       .primitiveFragmentShadingRateMeshShader = false,
       .meshShaderQueries = false,
       .hostImageCopy = false,
       .samplerYCbCrConversion = false,
       .dynamicRendering = false,
       .dynamicRenderingLocalRead = false,
   };

#if VK_KHR_fragment_shading_rate
   features.pipelineFragmentShadingRate = static_cast<bool>(fragmentShadingRateFeatures.pipelineFragmentShadingRate);
   features.primitiveFragmentShadingRate = static_cast<bool>(fragmentShadingRateFeatures.primitiveFragmentShadingRate);
   features.attachmentFragmentShadingRate = static_cast<bool>(fragmentShadingRateFeatures.attachmentFragmentShadingRate);
#endif

#if VK_KHR_acceleration_structure
   features.accelerationStructures = static_cast<bool>(accelerationStructureFeaturesKhr.accelerationStructure);
#endif

#if VK_KHR_ray_tracing_pipeline
   features.rayTracingPipeline = static_cast<bool>(rayTracingPipelineFeaturesKhr.rayTracingPipeline);
   features.rayTracingPipelineShaderGroupHandleCaptureReplay = static_cast<bool>(rayTracingPipelineFeaturesKhr.rayTracingPipelineShaderGroupHandleCaptureReplay);
   features.rayTracingPipelineShaderGroupHandleCaptureReplayMixed = static_cast<bool>(rayTracingPipelineFeaturesKhr.rayTracingPipelineShaderGroupHandleCaptureReplayMixed);
   features.rayTracingPipelineTraceRaysIndirect = static_cast<bool>(rayTracingPipelineFeaturesKhr.rayTracingPipelineTraceRaysIndirect);
   features.rayTraversalPrimitiveCulling = static_cast<bool>(rayTracingPipelineFeaturesKhr.rayTraversalPrimitiveCulling);
#endif

#if VK_EXT_mesh_shader
   features.taskShader = static_cast<bool>(meshShaderFeatures.taskShader);
   features.meshShader = static_cast<bool>(meshShaderFeatures.meshShader);
   features.multiviewMeshShader = static_cast<bool>(meshShaderFeatures.multiviewMeshShader);
   features.meshShaderQueries = static_cast<bool>(meshShaderFeatures.meshShaderQueries);
   features.primitiveFragmentShadingRateMeshShader = static_cast<bool>(meshShaderFeatures.primitiveFragmentShadingRateMeshShader);
#endif

#if VK_KHR_synchronization2
   supportsSynchronization2 = synchronization2Features.synchronization2;
#endif

#if VK_EXT_host_image_copy
   features.hostImageCopy = static_cast<bool>(hostImageCopyFeatures.hostImageCopy);
#endif

#if VK_KHR_sampler_ycbcr_conversion
   features.samplerYCbCrConversion = static_cast<bool>(ycbcrConversionFeatures.samplerYcbcrConversion);
#endif

#if VK_KHR_dynamic_rendering
   features.dynamicRendering = static_cast<bool>(dynamicRenderingFeatures.dynamicRendering);
#endif

#if VK_KHR_dynamic_rendering_local_read
   features.dynamicRenderingLocalRead = static_cast<bool>(dynamicLocalReadFeatures.dynamicRenderingLocalRead);
#endif

#if VK_KHR_swapchain_maintenance1
   features.swapchainMaintenance1 = swapchainMaintenanceFeatures.swapchainMaintenance1;
#endif
#if VK_KHR_timeline_semaphore
   features.timelineSemaphore = timelineSemaphoreFeatures.timelineSemaphore;
#endif

   return features;
}


std::vector<SM::AdapterQueueType> SM::Adapter::queryQueueFamily() {
    if(m_handle == VK_NULL_HANDLE) {
        SM_LOG_ERROR("RHI/Adapter", "Failed to query adapter's queue types, invalid adapter");
        return {};
    }    
    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &queueFamilyCount, nullptr);
        
    std::vector<VkQueueFamilyProperties> queueFamilies;
    queueFamilies.resize(queueFamilyCount);   
    vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &queueFamilyCount, queueFamilies.data());            

    for (uint32_t i = 0; i < queueFamilyCount; ++i) {
        const auto &queueFamily = queueFamilies[i];
        m_queueFamilies.emplace_back(SM::AdapterQueueType{
                .flags = queueFamily.queueFlags,
                .availableQueues = queueFamily.queueCount,
                .timestampValidBits = queueFamily.timestampValidBits,
                .minImageTransferGranularity = {
                    .width = queueFamily.minImageTransferGranularity.width,
                    .height = queueFamily.minImageTransferGranularity.height,
                    .depth = queueFamily.minImageTransferGranularity.depth } });
    }
    
    return m_queueFamilies;
}

bool SM::Adapter::supportsPresentation(const VkSurfaceKHR surfaceHandle, uint32_t queueFamilyIndex) const {
    if(surfaceHandle == VK_NULL_HANDLE) {
        SM_LOG_ERROR("RHI/Adapter", "Failed to check presentation support for queue {}, invalid surface m_handle", queueFamilyIndex);
        return {};
    }
    if(m_handle == VK_NULL_HANDLE) {
        SM_LOG_ERROR("RHI/Adapter", "Failed to check presentation support for queue {}, invalid adapter", queueFamilyIndex);
        return {};
    }
    
    VkBool32 canPresent = false;
    if(vkGetPhysicalDeviceSurfaceSupportKHR(m_handle, queueFamilyIndex, surfaceHandle, &canPresent) != VK_SUCCESS) {
        SM_LOG_ERROR("RHI/Adapter", "Failed to check presentation support for queue family index {}", queueFamilyIndex);
    }
    return canPresent;
}

SM::AdapterSwapchainProperties SM::Adapter::querySwapchainProperties(const VkSurfaceKHR &surfaceHandle)
{
    AdapterSwapchainProperties properties = {};

    // Get the capabilities
    if(surfaceHandle == VK_NULL_HANDLE) {
        SM_LOG_CRITICAL("RHI/Adapter", "Failed to query swapchain properties, invalid surface handle, aborting...");
        return {};
    }
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_handle, surfaceHandle, &capabilities);

    properties.capabilities = {
        .minImageCount = capabilities.minImageCount,
        .maxImageCount = capabilities.maxImageCount,
        .currentExtent = { capabilities.currentExtent.width, capabilities.currentExtent.height },
        .minImageExtent = { capabilities.minImageExtent.width, capabilities.minImageExtent.height },
        .maxImageExtent = { capabilities.maxImageExtent.width, capabilities.maxImageExtent.height },
        .maxImageArrayLayers = capabilities.maxImageArrayLayers,
        .supportedTransforms = capabilities.supportedTransforms,
        .currentTransform = capabilities.currentTransform,
        .supportedCompositeAlpha = capabilities.supportedCompositeAlpha,
        .supportedUsageFlags = capabilities.supportedUsageFlags
    };

    // Get the supported formats and colorspaces
    uint32_t formatCount = 0;
    std::vector<VkSurfaceFormatKHR> vkFormats;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_handle, surfaceHandle, &formatCount, nullptr);
    if (formatCount != 0) {
        vkFormats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_handle, surfaceHandle, &formatCount, vkFormats.data());
    }

    std::vector<SurfaceFormat> formats;
    formats.reserve(formatCount);
    for (uint32_t i = 0; i < formatCount; ++i) {
        formats.emplace_back(SurfaceFormat{
                vkFormats[i].format,
                vkFormats[i].colorSpace });
    }
    properties.formats = std::move(formats);

    // Get the supported present modes
    uint32_t presentModeCount = 0;
    std::vector<VkPresentModeKHR> vkPresentModes;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_handle, surfaceHandle, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        vkPresentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_handle, surfaceHandle, &presentModeCount, vkPresentModes.data());
    }

    std::vector<VkPresentModeKHR> presentModes;
    presentModes.reserve(presentModeCount);
    for (uint32_t i = 0; i < presentModeCount; ++i)
        presentModes.emplace_back(vkPresentModes[i]);
    properties.presentModes = std::move(presentModes);

    return properties;
}
