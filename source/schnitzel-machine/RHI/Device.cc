#include "RHI/Device.h"

#include <vulkan/vulkan_core.h>

#include "RHI/QueueDescription.h"
#include "RHI/ExtensionProperties.h"
#include "RHI/QueueFamilyProperties.h"
#include "RHI/VulkanConfig.h"
#include "core/SMResult.h"
#include "core/Log.h"
#include "RHI/VulkanConfig.h"
        
void SM::Device::initialize(const SM::Adapter &adapter, const SM::DeviceOptions &options, std::vector<QueueRequest> &queueRequests) {    
    // Merge requested device extensions and layers with our defaults
    const auto availableDeviceExtensions = adapter.extensions();
    std::vector<const char *> requestedDeviceExtensions;
    auto defaultRequestedDeviceExtensions = getDefaultRequestedDeviceExtensions();

    // Add requested device extensions set by user in the optionsxp
    for (const std::string &userRequestedExtension : options.extensions) {
        defaultRequestedDeviceExtensions.push_back(userRequestedExtension.c_str());        
    }

    for (const char *requestedDeviceExtension : defaultRequestedDeviceExtensions) {
        if (SM::hasExtension(availableDeviceExtensions, requestedDeviceExtension)) {
            requestedDeviceExtensions.push_back(requestedDeviceExtension);
        } else {
            SM_LOG_WARN("RHI/Device", "Unable to find default requested device extension {}", requestedDeviceExtension);
        }
    }

    // This makes it easier to chain pNext pointers in the device createInfo struct especially when we
    // have a lot of them and some of them are optional.
    VkBaseOutStructure *chainCurrent{ nullptr };
    auto addToChain = [&chainCurrent](auto *next) {
        auto *n = reinterpret_cast<VkBaseOutStructure *>(next);
        chainCurrent->pNext = n;
        chainCurrent = n;
    };

    queueRequests = options.queues;
    if (queueRequests.empty()) {
        QueueRequest queueRequest = {
            .familyIndex = 0,
            .count = 1,
            .priorities = { 1.0f }
        };
        queueRequests.emplace_back(queueRequest);
    }

    uint32_t queueRequestCount = queueRequests.size();
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    queueCreateInfos.reserve(queueRequestCount);
    // Creating device queue info for each queue family   
    for (const auto &queueRequest : queueRequests) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueRequest.familyIndex;
        queueCreateInfo.queueCount       = queueRequest.count;
        queueCreateInfo.pQueuePriorities = queueRequest.priorities.data();

        queueCreateInfos.push_back(queueCreateInfo);
    }

    // Request the physical device features requested by options
    VkPhysicalDeviceFeatures deviceFeatures = {};
    {
        deviceFeatures.robustBufferAccess = options.requestedFeatures.robustBufferAccess;
        deviceFeatures.fullDrawIndexUint32 = options.requestedFeatures.fullDrawIndexUint32;
        deviceFeatures.imageCubeArray = options.requestedFeatures.imageCubeArray;
        deviceFeatures.independentBlend = options.requestedFeatures.independentBlend;
        deviceFeatures.geometryShader = options.requestedFeatures.geometryShader;
        deviceFeatures.tessellationShader = options.requestedFeatures.tessellationShader;
        deviceFeatures.sampleRateShading = options.requestedFeatures.sampleRateShading;
        deviceFeatures.dualSrcBlend = options.requestedFeatures.dualSrcBlend;
        deviceFeatures.logicOp = options.requestedFeatures.logicOp;
        deviceFeatures.multiDrawIndirect = options.requestedFeatures.multiDrawIndirect;
        deviceFeatures.drawIndirectFirstInstance = options.requestedFeatures.drawIndirectFirstInstance;
        deviceFeatures.depthClamp = options.requestedFeatures.depthClamp;
        deviceFeatures.depthBiasClamp = options.requestedFeatures.depthBiasClamp;
        deviceFeatures.fillModeNonSolid = options.requestedFeatures.fillModeNonSolid;
        deviceFeatures.depthBounds = options.requestedFeatures.depthBounds;
        deviceFeatures.wideLines = options.requestedFeatures.wideLines;
        deviceFeatures.largePoints = options.requestedFeatures.largePoints;
        deviceFeatures.alphaToOne = options.requestedFeatures.alphaToOne;
        deviceFeatures.multiViewport = options.requestedFeatures.multiViewport;
        deviceFeatures.samplerAnisotropy = options.requestedFeatures.samplerAnisotropy;
        deviceFeatures.textureCompressionETC2 = options.requestedFeatures.textureCompressionETC2;
        deviceFeatures.textureCompressionASTC_LDR = options.requestedFeatures.textureCompressionASTC_LDR;
        deviceFeatures.textureCompressionBC = options.requestedFeatures.textureCompressionBC;
        deviceFeatures.occlusionQueryPrecise = options.requestedFeatures.occlusionQueryPrecise;
        deviceFeatures.pipelineStatisticsQuery = options.requestedFeatures.pipelineStatisticsQuery;
        deviceFeatures.vertexPipelineStoresAndAtomics = options.requestedFeatures.vertexPipelineStoresAndAtomics;
        deviceFeatures.fragmentStoresAndAtomics = options.requestedFeatures.fragmentStoresAndAtomics;
        deviceFeatures.shaderTessellationAndGeometryPointSize = options.requestedFeatures.shaderTessellationAndGeometryPointSize;
        deviceFeatures.shaderImageGatherExtended = options.requestedFeatures.shaderImageGatherExtended;
        deviceFeatures.shaderStorageImageExtendedFormats = options.requestedFeatures.shaderStorageImageExtendedFormats;
        deviceFeatures.shaderStorageImageMultisample = options.requestedFeatures.shaderStorageImageMultisample;
        deviceFeatures.shaderStorageImageReadWithoutFormat = options.requestedFeatures.shaderStorageImageReadWithoutFormat;
        deviceFeatures.shaderStorageImageWriteWithoutFormat = options.requestedFeatures.shaderStorageImageWriteWithoutFormat;
        deviceFeatures.shaderUniformBufferArrayDynamicIndexing = options.requestedFeatures.shaderUniformBufferArrayDynamicIndexing;
        deviceFeatures.shaderSampledImageArrayDynamicIndexing = options.requestedFeatures.shaderSampledImageArrayDynamicIndexing;
        deviceFeatures.shaderStorageBufferArrayDynamicIndexing = options.requestedFeatures.shaderStorageBufferArrayDynamicIndexing;
        deviceFeatures.shaderStorageImageArrayDynamicIndexing = options.requestedFeatures.shaderStorageImageArrayDynamicIndexing;
        deviceFeatures.shaderClipDistance = options.requestedFeatures.shaderClipDistance;
        deviceFeatures.shaderCullDistance = options.requestedFeatures.shaderCullDistance;
        deviceFeatures.shaderFloat64 = options.requestedFeatures.shaderFloat64;
        deviceFeatures.shaderInt64 = options.requestedFeatures.shaderInt64;
        deviceFeatures.shaderInt16 = options.requestedFeatures.shaderInt16;
        deviceFeatures.shaderResourceResidency = options.requestedFeatures.shaderResourceResidency;
        deviceFeatures.shaderResourceMinLod = options.requestedFeatures.shaderResourceMinLod;
        deviceFeatures.sparseBinding = options.requestedFeatures.sparseBinding;
        deviceFeatures.sparseResidencyBuffer = options.requestedFeatures.sparseResidencyBuffer;
        deviceFeatures.sparseResidencyImage2D = options.requestedFeatures.sparseResidencyImage2D;
        deviceFeatures.sparseResidencyImage3D = options.requestedFeatures.sparseResidencyImage3D;
        deviceFeatures.sparseResidency2Samples = options.requestedFeatures.sparseResidency2Samples;
        deviceFeatures.sparseResidency4Samples = options.requestedFeatures.sparseResidency4Samples;
        deviceFeatures.sparseResidency8Samples = options.requestedFeatures.sparseResidency8Samples;
        deviceFeatures.sparseResidency16Samples = options.requestedFeatures.sparseResidency16Samples;
        deviceFeatures.sparseResidencyAliased = options.requestedFeatures.sparseResidencyAliased;
        deviceFeatures.variableMultisampleRate = options.requestedFeatures.variableMultisampleRate;
        deviceFeatures.inheritedQueries = options.requestedFeatures.inheritedQueries;
    }

    // Some newer features we have to request via VkPhysicalDeviceFeatures2
    VkPhysicalDeviceFeatures2 physicalDeviceFeatures2 = {};
    physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    physicalDeviceFeatures2.features = deviceFeatures;

    // Start the chain
    chainCurrent = reinterpret_cast<VkBaseOutStructure *>(&physicalDeviceFeatures2);

    // Allows to use std430 for uniform buffers which gives much nicer packing of data
    VkPhysicalDeviceUniformBufferStandardLayoutFeatures stdLayoutFeatures = {};
    stdLayoutFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES;
    stdLayoutFeatures.uniformBufferStandardLayout = options.requestedFeatures.uniformBufferStandardLayout;
    addToChain(&stdLayoutFeatures);

    // Enable multiview rendering if requested
    VkPhysicalDeviceMultiviewFeatures multiViewFeatures{};
    multiViewFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES;
    multiViewFeatures.multiview = options.requestedFeatures.multiView;
    multiViewFeatures.multiviewGeometryShader = options.requestedFeatures.multiViewGeometryShader;
    multiViewFeatures.multiviewTessellationShader = options.requestedFeatures.multiViewTessellationShader;
    addToChain(&multiViewFeatures);

    // Enable Descriptor Indexing if requested
    VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures{};
    descriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
    descriptorIndexingFeatures.shaderInputAttachmentArrayDynamicIndexing = options.requestedFeatures.shaderInputAttachmentArrayDynamicIndexing;
    descriptorIndexingFeatures.shaderUniformTexelBufferArrayDynamicIndexing = options.requestedFeatures.shaderUniformTexelBufferArrayDynamicIndexing;
    descriptorIndexingFeatures.shaderStorageTexelBufferArrayDynamicIndexing = options.requestedFeatures.shaderStorageTexelBufferArrayDynamicIndexing;
    descriptorIndexingFeatures.shaderUniformBufferArrayNonUniformIndexing = options.requestedFeatures.shaderUniformBufferArrayNonUniformIndexing;
    descriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing = options.requestedFeatures.shaderSampledImageArrayNonUniformIndexing;
    descriptorIndexingFeatures.shaderStorageBufferArrayNonUniformIndexing = options.requestedFeatures.shaderStorageBufferArrayNonUniformIndexing;
    descriptorIndexingFeatures.shaderStorageImageArrayNonUniformIndexing = options.requestedFeatures.shaderStorageImageArrayNonUniformIndexing;
    descriptorIndexingFeatures.shaderInputAttachmentArrayNonUniformIndexing = options.requestedFeatures.shaderInputAttachmentArrayNonUniformIndexing;
    descriptorIndexingFeatures.shaderUniformTexelBufferArrayNonUniformIndexing = options.requestedFeatures.shaderUniformTexelBufferArrayNonUniformIndexing;
    descriptorIndexingFeatures.shaderStorageTexelBufferArrayNonUniformIndexing = options.requestedFeatures.shaderStorageTexelBufferArrayNonUniformIndexing;
    descriptorIndexingFeatures.descriptorBindingUniformBufferUpdateAfterBind = options.requestedFeatures.bindGroupBindingUniformBufferUpdateAfterBind;
    descriptorIndexingFeatures.descriptorBindingSampledImageUpdateAfterBind = options.requestedFeatures.bindGroupBindingSampledImageUpdateAfterBind;
    descriptorIndexingFeatures.descriptorBindingStorageImageUpdateAfterBind = options.requestedFeatures.bindGroupBindingStorageImageUpdateAfterBind;
    descriptorIndexingFeatures.descriptorBindingStorageBufferUpdateAfterBind = options.requestedFeatures.bindGroupBindingStorageBufferUpdateAfterBind;
    descriptorIndexingFeatures.descriptorBindingUniformTexelBufferUpdateAfterBind = options.requestedFeatures.bindGroupBindingUniformTexelBufferUpdateAfterBind;
    descriptorIndexingFeatures.descriptorBindingStorageTexelBufferUpdateAfterBind = options.requestedFeatures.bindGroupBindingStorageTexelBufferUpdateAfterBind;
    descriptorIndexingFeatures.descriptorBindingUpdateUnusedWhilePending = options.requestedFeatures.bindGroupBindingUpdateUnusedWhilePending;
    descriptorIndexingFeatures.descriptorBindingPartiallyBound = options.requestedFeatures.bindGroupBindingPartiallyBound;
    descriptorIndexingFeatures.descriptorBindingVariableDescriptorCount = options.requestedFeatures.bindGroupBindingVariableDescriptorCount;
    descriptorIndexingFeatures.runtimeDescriptorArray = options.requestedFeatures.runtimeBindGroupArray;
    addToChain(&descriptorIndexingFeatures);

    // Create a Device that targets several physical devices if a group was specified.
    // We only add the device group info if we have more than one adapter.
    VkDeviceGroupDeviceCreateInfo deviceGroupInfo = {};
    deviceGroupInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO_KHR;
    deviceGroupInfo.physicalDeviceCount = 0;

    VkPhysicalDeviceBufferDeviceAddressFeatures bufferDeviceFeature{};
    bufferDeviceFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
    bufferDeviceFeature.bufferDeviceAddress = options.requestedFeatures.bufferDeviceAddress;
    addToChain(&bufferDeviceFeature);

#if VK_KHR_fragment_shading_rate
    VkPhysicalDeviceFragmentShadingRateFeaturesKHR fragmentShadingRateFeatures{};
    fragmentShadingRateFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR;
    fragmentShadingRateFeatures.pipelineFragmentShadingRate = options.requestedFeatures.pipelineFragmentShadingRate;
    fragmentShadingRateFeatures.primitiveFragmentShadingRate = options.requestedFeatures.primitiveFragmentShadingRate;
    fragmentShadingRateFeatures.attachmentFragmentShadingRate = static_cast<bool>(options.requestedFeatures.attachmentFragmentShadingRate);
    addToChain(&fragmentShadingRateFeatures);
#endif

#if VK_KHR_acceleration_structure
    VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeaturesKhr{};
    if (options.requestedFeatures.accelerationStructures) {
        // Enable raytracing acceleration structure
        accelerationStructureFeaturesKhr.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        accelerationStructureFeaturesKhr.accelerationStructure = options.requestedFeatures.accelerationStructures;
        addToChain(&accelerationStructureFeaturesKhr);
    }
#endif

#if VK_KHR_ray_tracing_pipeline
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR raytracingFeaturesKhr{};

    // When running with RenderDoc (as of 1.39) it appears that options.requestedFeatures.rayTracingPipeline returns true
    // even though VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME is not present in the list of supported runtime extensions
    // (they've likely not handled all checks in their VulkanLoader)
    // Launching an application with RenderDoc then fails at device creation (returning unsupported features). To bypass
    // this, we add an extra check to see if the extension is in the list of runtime supported extensions.
    const bool deviceHasRuntimeRayTracingPipelineExtension = hasExtension(requestedDeviceExtensions, VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
    if (options.requestedFeatures.rayTracingPipeline && deviceHasRuntimeRayTracingPipelineExtension) {
        // Enable raytracing pipelines
        raytracingFeaturesKhr.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
        raytracingFeaturesKhr.rayTracingPipeline = options.requestedFeatures.rayTracingPipeline;
        raytracingFeaturesKhr.rayTracingPipelineShaderGroupHandleCaptureReplay = options.requestedFeatures.rayTracingPipelineShaderGroupHandleCaptureReplay;
        raytracingFeaturesKhr.rayTracingPipelineShaderGroupHandleCaptureReplayMixed = options.requestedFeatures.rayTracingPipelineShaderGroupHandleCaptureReplayMixed;
        raytracingFeaturesKhr.rayTracingPipelineTraceRaysIndirect = options.requestedFeatures.rayTracingPipelineTraceRaysIndirect;
        raytracingFeaturesKhr.rayTraversalPrimitiveCulling = options.requestedFeatures.rayTraversalPrimitiveCulling;
        addToChain(&raytracingFeaturesKhr);
    }
#endif

#if VK_EXT_mesh_shader
    VkPhysicalDeviceMeshShaderFeaturesEXT meshShaderFeatures{};
    if (options.requestedFeatures.meshShader) {
        // Enable Mesh/Task shading
        meshShaderFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;
        meshShaderFeatures.taskShader = options.requestedFeatures.taskShader;
        meshShaderFeatures.meshShader = options.requestedFeatures.meshShader;
        meshShaderFeatures.multiviewMeshShader = options.requestedFeatures.multiviewMeshShader;
#if VK_KHR_fragment_shading_rate
        // Requires to have VkPhysicalDeviceFragmentShadingRateFeaturesKHR enabled
        meshShaderFeatures.primitiveFragmentShadingRateMeshShader =
            options.requestedFeatures.primitiveFragmentShadingRate &&
            options.requestedFeatures.primitiveFragmentShadingRateMeshShader;
#else
        meshShaderFeatures.primitiveFragmentShadingRateMeshShader = false;
#endif
        meshShaderFeatures.meshShaderQueries = options.requestedFeatures.meshShaderQueries;
        addToChain(&meshShaderFeatures);
    }
#endif

#if VK_EXT_host_image_copy
    VkPhysicalDeviceHostImageCopyFeaturesEXT hostImageCopyFeatures{};
    if (options.requestedFeatures.hostImageCopy) {
        // Enable HostImage copy
        hostImageCopyFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_FEATURES_EXT;
        hostImageCopyFeatures.hostImageCopy = options.requestedFeatures.hostImageCopy;
        addToChain(&hostImageCopyFeatures);
    }
#endif

#if VK_KHR_sampler_ycbcr_conversion
    VkPhysicalDeviceSamplerYcbcrConversionFeaturesKHR ycbcrConversionFeatures{};
    if (options.requestedFeatures.samplerYCbCrConversion) {
        // Enable yCbCr sampler conversion
        ycbcrConversionFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES_KHR;
        ycbcrConversionFeatures.samplerYcbcrConversion = options.requestedFeatures.samplerYCbCrConversion;
        addToChain(&ycbcrConversionFeatures);
    }
#endif

    // std::vector<VkPhysicalDevice> devicesInGroup;
    // // const size_t adapterCount = options.adapterGroup.adapters.size();
    // const size_t adapterCount = 0; //
    // const bool useDeviceGroup = adapterCount > 1;

    // if (useDeviceGroup) {
    //     // Fetch VkPhysicalDevice from Handle<Adapter_t>
    //     devicesInGroup.reserve(adapterCount);
    //     for (const Handle<Adapter_t> &h : options.adapterGroup.adapters) {
    //         VulkanAdapter *adapter = getAdapter(h);
    //         assert(adapter);
    //         devicesInGroup.emplace_back(adapter->physicalDevice);
    //     }

    //     deviceGroupInfo.physicalDeviceCount = options.adapterGroup.adapters.size();
    //     deviceGroupInfo.pPhysicalDevices = devicesInGroup.data();

    //     addToChain(&deviceGroupInfo);
    // }

#if VK_KHR_synchronization2
    // Enable the VK_KHR_Synchronization2 extension features by chaining this into the createInfo chain.
    VkPhysicalDeviceSynchronization2FeaturesKHR sync2Features = {};
    sync2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR;
    sync2Features.synchronization2 = adapter.features().supportsSynchronization2;
    addToChain(&sync2Features);
#endif

#if VK_KHR_dynamic_rendering
    VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeatures{};
    if (options.requestedFeatures.dynamicRendering) {
        // Enable dynamic rendering
        dynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
        dynamicRenderingFeatures.dynamicRendering = options.requestedFeatures.dynamicRendering;
        addToChain(&dynamicRenderingFeatures);
    }
#endif

#if VK_KHR_dynamic_rendering_local_read
    VkPhysicalDeviceDynamicRenderingLocalReadFeaturesKHR dynamicLocalReadFeatures{};
    if (options.requestedFeatures.dynamicRenderingLocalRead) {
        dynamicLocalReadFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_LOCAL_READ_FEATURES_KHR;
        dynamicLocalReadFeatures.dynamicRenderingLocalRead = static_cast<bool>(options.requestedFeatures.dynamicRenderingLocalRead);
        addToChain(&dynamicLocalReadFeatures);
    }
#endif

#if VK_KHR_swapchain_maintenance1
    VkPhysicalDeviceSwapchainMaintenance1FeaturesKHR swapchainMaintenanceFeatures{};
    if (options.requestedFeatures.swapchainMaintenance1) {
        swapchainMaintenanceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SWAPCHAIN_MAINTENANCE_1_FEATURES_KHR;
        swapchainMaintenanceFeatures.swapchainMaintenance1 = static_cast<bool>(options.requestedFeatures.swapchainMaintenance1);
        addToChain(&swapchainMaintenanceFeatures);
    }
#endif

#if VK_KHR_timeline_semaphore
    VkPhysicalDeviceTimelineSemaphoreFeaturesKHR timelineSemaphoreFeatures{};
    timelineSemaphoreFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR;
    timelineSemaphoreFeatures.timelineSemaphore = options.requestedFeatures.timelineSemaphore;
    addToChain(&timelineSemaphoreFeatures);
#endif

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = &physicalDeviceFeatures2;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = nullptr; // we use VkPhysicalDeviceFeatures2 set on pNext
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
    createInfo.enabledExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = nullptr;

    // check for Vulkan API support, fall back to extensions if needed
    auto maxApiVersionSupportedByPhysicalDevice = adapter.properties().apiVersion;
    auto apiVersion = options.apiVersion;
    SM_LOG_INFO("RHI/Device", "Requested Vulkan API Version {}.{}.{}",
                VK_VERSION_MAJOR(apiVersion), VK_VERSION_MINOR(apiVersion), VK_VERSION_PATCH(apiVersion));
    
    SM_LOG_INFO("RHI/Device", "Physical Device supports Vulkan API Version {}.{}.{}",
                VK_VERSION_MAJOR(maxApiVersionSupportedByPhysicalDevice),
                VK_VERSION_MINOR(maxApiVersionSupportedByPhysicalDevice),
                VK_VERSION_PATCH(maxApiVersionSupportedByPhysicalDevice));
    
    if (maxApiVersionSupportedByPhysicalDevice < apiVersion) {
        SM_LOG_WARN("RHI/Device", "Downgrading requested Vulkan API Version {}.{}.{} because physical device only supports {}.{}.{}",
                    VK_VERSION_MAJOR(apiVersion), VK_VERSION_MINOR(apiVersion), VK_VERSION_PATCH(apiVersion),
                    VK_VERSION_MAJOR(maxApiVersionSupportedByPhysicalDevice),
                    VK_VERSION_MINOR(maxApiVersionSupportedByPhysicalDevice),
                    VK_VERSION_PATCH(maxApiVersionSupportedByPhysicalDevice));
        apiVersion = maxApiVersionSupportedByPhysicalDevice;
    }
    // const bool hasVulkan12 = apiVersion >= VK_API_VERSION_1_2;
    const bool hasVulkan11 = apiVersion >= VK_API_VERSION_1_1;
    if (!hasVulkan11) {
        SM_LOG_CRITICAL("RHI/Device", "At least Vulkan 1.1 is required!");
        abort();
    }

    if (!requestedDeviceExtensions.empty()) {
        createInfo.enabledExtensionCount = static_cast<uint32_t>(requestedDeviceExtensions.size());
        // assert(requestedDeviceExtensions.size() <= std::numeric_limits<uint32_t>::max());
        createInfo.ppEnabledExtensionNames = requestedDeviceExtensions.data();
    }

    if(auto result = vkCreateDevice(adapter.getHandle(), &createInfo, nullptr, &m_handle); result != VK_SUCCESS) {
        SM_LOG_CRITICAL("RHI/Device", "{}: Failed to create device", SM::toString(result));
    }   
}

std::vector<SM::QueueDescription> SM::Device::getQueues(const std::vector<QueueRequest> &queueRequests, const std::vector<SM::QueueFamilyProperties> &queueProperties)
{ 
    uint32_t queueCount = 0;
    for (const auto &queueRequest : queueRequests)
        queueCount += queueRequest.count;
    
    m_queueDescriptions.clear();
    m_queueDescriptions.reserve(queueCount);

    for (const auto &queueRequest : queueRequests) {
        const uint32_t queueCountForFamily = queueRequest.count;
        for (uint32_t j = 0; j < queueCountForFamily; ++j) {
            VkQueue vkQueue{ VK_NULL_HANDLE };
            vkGetDeviceQueue(m_handle, queueRequest.familyIndex, j, &vkQueue);
            // m_queues.emplace_back(vkQueue);

            QueueDescription queueDescription{
                .queue = vkQueue,
                .flags = queueProperties[queueRequest.familyIndex].flags,
                .timestampValidBits = queueProperties[queueRequest.familyIndex].timestampValidBits,
                .minImageTransferGranularity = queueProperties[queueRequest.familyIndex].minImageTransferGranularity,
                .familyIndex = queueRequest.familyIndex
            };
            m_queueDescriptions.push_back(queueDescription);
        }
    }

    return m_queueDescriptions;
}

void SM::Device::destroy() {
    if (m_handle != VK_NULL_HANDLE) {
        vkDestroyDevice(m_handle, nullptr);
    }
}
        
