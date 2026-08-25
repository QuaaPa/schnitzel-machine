#include "RHI/VulkanRHI.h"

#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#include "RHI/Adapter.h"
#include "RHI/Device.h"
#include "RHI/Instance.h"
#include "RHI/Queue.h"
#include "RHI/Swapchain.h"
#include "core/TypesDefs.h"
#include "core/Log.h"
#include "RHI/Queue.h"
#include "RHI/AdapterSwapchainProperties.h"

VkInstance SM::VulkanRHI::createInstance(const SM::InstanceOptions& options) {
    if(m_instance.initialize(options) != SM_SUCCESS) {
        SM_LOG_CRITICAL("RHI", "Failed to initialize instance");
        return {};
    }
    return m_instance.getHandle();
}

VkSurfaceKHR SM::VulkanRHI::createSurface(const SM::WindowHandle &window) {
    // Release window creation for diffrent window type
    //
    if(m_surface.initialize(window, m_instance.getHandle()) != SM_SUCCESS) {
        SM_LOG_CRITICAL("RHI", "Failed to initialize surface");
        return {};
    }
    return m_surface.getHandle();    
}

VkPhysicalDevice SM::VulkanRHI::selectAdapter() {
    auto getAdapterScore = [](SM::Adapter adapter) {
        switch (adapter.properties().deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            return 1000; // the best choise for rendering
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            return 500;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            return 100;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            return 10; 
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        default:
            return 0;
        }
    };
    
    SM::Adapter best;
    int adapterBestScore = 0;
    
    for (auto adapter : m_instance.queryAdapters()) {                
        if(getAdapterScore(adapter) >= adapterBestScore) {
            best = adapter;
        }                
    }
    m_adapter = best;
    
    SM_LOG_DEBUG("RHI", "Selected adapter: {}", m_adapter.properties().deviceName);
    if (m_adapter.getHandle() == VK_NULL_HANDLE) {
        SM_LOG_CRITICAL("RHI", "Unable to find a suitable Adapter. Aborting...");
        return {};
    }    
    return m_adapter.getHandle();    
}

VkDevice SM::VulkanRHI::createDevice(const SM::DeviceOptions &options) {
    const auto queueFamilies = m_adapter.queryQueueFamily();

    SM_LOG_INFO("RHI", "Found {} queue famil{}", queueFamilies.size(), queueFamilies.size() == 1 ? "y" : "ies");

    const bool supportsPresentation = m_adapter.supportsPresentation(m_surface.getHandle(), 0); 
    const bool hasGraphicsAndCompute = queueFamilies[0].supportsFeature(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
    
    for (uint32_t i = 0; i < queueFamilies.size(); ++i) {
        const auto &family = queueFamilies[i];

        const bool graphics       = family.supportsFeature(VK_QUEUE_GRAPHICS_BIT);
        const bool compute         = family.supportsFeature(VK_QUEUE_COMPUTE_BIT);
        const bool transfer        = family.supportsFeature(VK_QUEUE_TRANSFER_BIT);
        const bool sparseBinding   = family.supportsFeature(VK_QUEUE_SPARSE_BINDING_BIT);
        const bool videoDecode     = family.supportsFeature(VK_QUEUE_VIDEO_DECODE_BIT_KHR);
#if VK_ENABLE_BETA_EXTENSIONS
        const bool videoEncode     = family.supportsFeature(VK_QUEUE_VIDEO_ENCODE_BIT_KHR);
#endif
        const bool opticalFlow     = family.supportsFeature(VK_QUEUE_OPTICAL_FLOW_BIT_NV);

        const bool presentation    = m_adapter.supportsPresentation(m_surface.getHandle(), i);

        SM_LOG_DEBUG("RHI", "Queue family {}:", i);
        SM_LOG_DEBUG("RHI", "  - queueCount: {}", family.availableQueues); 
        SM_LOG_DEBUG("RHI", "  - Graphics:        {}", graphics);
        SM_LOG_DEBUG("RHI", "  - Compute:         {}", compute);
        SM_LOG_DEBUG("RHI", "  - Transfer:        {}", transfer);
        SM_LOG_DEBUG("RHI", "  - Sparse binding:  {}", sparseBinding);
        SM_LOG_DEBUG("RHI", "  - Video decode:    {}", videoDecode);
#if VK_ENABLE_BETA_EXTENSIONS
        SM_LOG_DEBUG("RHI", "  - Video encode:    {}", videoEncode);
#endif
        SM_LOG_DEBUG("RHI", "  - Optical flow:    {}", opticalFlow);
        SM_LOG_DEBUG("RHI", "  - Presentation:    {}", presentation);
    }
    // We are now able to query the adapter for swapchain properties and presentation support with the window surface
    const auto swapchainProperties = m_adapter.querySwapchainProperties(m_surface.getHandle());
    SM_LOG_INFO("RHI", "Swapchain support {} present mode:", swapchainProperties.presentModes.size());
    for (const auto &mode : swapchainProperties.presentModes) {
        SM_LOG_DEBUG("RHI", "  - {}", SM::presentModeToString(mode));
    }

    const auto adapterExtensions = m_adapter.extensions();
    SM_LOG_INFO("RHI", "Adapter has {} available extensions:", adapterExtensions.size());
    for (const auto &extension : adapterExtensions) {
        SM_LOG_DEBUG("RHI", "  - {} Version {}", extension.extensionName, extension.specVersion);
    }

    if (!supportsPresentation || !hasGraphicsAndCompute) {
        SM_LOG_CRITICAL("RHI", "Selected adapter queue family 0 does not meet requirements. Aborting.");
        return {};
    }

    const bool supportsMultiView = m_adapter.features().multiView;
    SM_LOG_INFO("RHI", "Supports multiview: {}", supportsMultiView);

    const bool supportsUBOIndexing = m_adapter.features().shaderUniformBufferArrayNonUniformIndexing && m_adapter.features().bindGroupBindingUniformBufferUpdateAfterBind;
    SM_LOG_INFO("RHI", "Supports Uniform Bind Group Dynamic Indexing: {}", supportsUBOIndexing);

    const bool supportsAccelerationStructures = m_adapter.features().accelerationStructures;
    SM_LOG_INFO("RHI", "Supports acceleration structures: {}", supportsAccelerationStructures);

    const bool supportsRayTracing = m_adapter.features().rayTracingPipeline;
    SM_LOG_INFO("RHI", "Supports raytracing: {}", supportsRayTracing);

    const bool supportsMeshShader = m_adapter.features().meshShader;
    const bool supportsTaskShader = m_adapter.features().taskShader;
    SM_LOG_INFO("RHI", "Supports meshShader: {}", supportsMeshShader);
    SM_LOG_INFO("RHI", "Supports taskShader: {}", supportsTaskShader);

    const bool supportsHostToImageCopy = m_adapter.features().hostImageCopy;
    SM_LOG_INFO("RHI", "Supports host to image copy: {}", supportsHostToImageCopy);

    // Now we can create a device from the selected adapter that we can then use to interact with the GPU.
    std::vector<QueueRequest> queueRequests;
    m_device.initialize(m_adapter,
                        DeviceOptions{},
                        queueRequests);
        
    std::vector<QueueDescription>  queueDescriptions = m_device.getQueues(queueRequests, queueFamilies);

    const uint32_t queueCount = queueDescriptions.size();
    m_queues.reserve(queueCount);
    for (uint32_t i = 0; i < queueCount; ++i) {
        m_queues.emplace_back(SM::Queue(m_device.getHandle(), queueDescriptions[i]));           
    }
    
    return m_device.getHandle();
}

VkSwapchainKHR SM::VulkanRHI::createSwapchain(const SM::SwapchainOptions& options) {
    if(m_swapchain.initialize(m_adapter, m_device.getHandle(), options) != SM_SUCCESS) {
        SM_LOG_CRITICAL("RHI", "Failed to initialize swapchain");
    }
    return m_swapchain.getHandle();
}

SM::SMResult SM::VulkanRHI::destroy() {
    SM_LOG_DEBUG("RHI", "Waiting for a device to become idle, before RHI destroying...");
    // vkDeviceWaitIdle(m_device.getHandle());

    // reseting vulkan objects...
    // in correct order
    m_swapchain.destroy();
    m_device.destroy();
    m_surface.destroy();
    m_instance.destroy();
    
    return SM_SUCCESS;
}
