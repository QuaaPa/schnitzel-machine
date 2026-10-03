#include "RHI/VulkanRHI.h"

#include <cstdint>
#include <initializer_list>
#include <memory>
#include <vector>

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#include "RHI/AdapterFeatures.h"
#include "RHI/Surface.h"
#include "RHI/Instance.h"
#include "RHI/Adapter.h"
#include "RHI/Device.h"
#include "core/Log.h"

static void logAdapterSupportability(const SM::Adapter *pAdapter, const SM::Surface *pSurface) {
    const auto queueFamilyProperties = pAdapter->queryQueueFamilyProperties();
    SM_LOG_INFO("RHI", "Found {} queue famil{}", queueFamilyProperties.size(), queueFamilyProperties.size() == 1 ? "y" : "ies");
    for (uint32_t i = 0; i < queueFamilyProperties.size(); ++i) {
        const auto& family = queueFamilyProperties[i];

        const bool graphics = family.supportsFeature(VK_QUEUE_GRAPHICS_BIT);
        const bool compute = family.supportsFeature(VK_QUEUE_COMPUTE_BIT);
        const bool transfer = family.supportsFeature(VK_QUEUE_TRANSFER_BIT);
        const bool sparseBinding = family.supportsFeature(VK_QUEUE_SPARSE_BINDING_BIT);
        const bool videoDecode = family.supportsFeature(VK_QUEUE_VIDEO_DECODE_BIT_KHR);
#if VK_ENABLE_BETA_EXTENSIONS
        const bool videoEncode = family.supportsFeature(VK_QUEUE_VIDEO_ENCODE_BIT_KHR);
#endif
        const bool opticalFlow = family.supportsFeature(VK_QUEUE_OPTICAL_FLOW_BIT_NV);

        const bool presentation = family.supportsPresentation(pAdapter->getHandle(), pSurface->getHandle());

        SM_LOG_DEBUG("RHI", "Queue family {}:"       , i);
        SM_LOG_DEBUG("RHI", "  - queueCount: {}"     , family.availableQueueCount);
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

    const auto swapchainProperties = pAdapter->querySwapchainProperties(pSurface->getHandle());
    SM_LOG_INFO("RHI", "Swapchain support {} present mode:", swapchainProperties.presentModes.size());
    for (const auto& mode : swapchainProperties.presentModes) {
        SM_LOG_DEBUG("RHI", "  - {}", SM::presentModeToString(mode));
    }

    const auto adapterExtensions = pAdapter->extensions();
    SM_LOG_INFO("RHI", "Adapter has {} available extensions:", adapterExtensions.size());
    for (const auto& extension : adapterExtensions) {
        SM_LOG_DEBUG("RHI", "  - {} Version {}", extension.extensionName, extension.specVersion);
    }

    const bool supportsMultiView = pAdapter->features().multiView;
    SM_LOG_INFO("RHI", "Supports multiview: {}", supportsMultiView);

    const bool supportsUBOIndexing = pAdapter->features().shaderUniformBufferArrayNonUniformIndexing && pAdapter->features().bindGroupBindingUniformBufferUpdateAfterBind;
    SM_LOG_INFO("RHI", "Supports Uniform Bind Group Dynamic Indexing: {}", supportsUBOIndexing);

    const bool supportsAccelerationStructures = pAdapter->features().accelerationStructures;
    SM_LOG_INFO("RHI", "Supports acceleration structures: {}", supportsAccelerationStructures);

    const bool supportsRayTracing = pAdapter->features().rayTracingPipeline;
    SM_LOG_INFO("RHI", "Supports raytracing: {}", supportsRayTracing);

    const bool supportsMeshShader = pAdapter->features().meshShader;
    const bool supportsTaskShader = pAdapter->features().taskShader;
    SM_LOG_INFO("RHI", "Supports meshShader: {}", supportsMeshShader);
    SM_LOG_INFO("RHI", "Supports taskShader: {}", supportsTaskShader);

    const bool supportsHostToImageCopy = pAdapter->features().hostImageCopy;
    SM_LOG_INFO("RHI", "Supports host to image copy: {}", supportsHostToImageCopy);
}

SM::VulkanRHI::VulkanRHI(uint32_t apiVersion, SM::WindowHandle windowHandle) {
    SM::InstanceOptions instanceOpt{};
    instanceOpt.applicationName = "SM_APPLICATION";
    instanceOpt.applicationVersion = SM_MAKE_VERSION(0, 0, 1);
    instanceOpt.engineVersion = SM_MAKE_VERSION(0, 0, 1);
    instanceOpt.extensions = { "VK_KHR_wayland_surface" };

    m_instance = std::make_unique<SM::Instance>(apiVersion, instanceOpt);

    m_surface = std::make_unique<SM::Surface>(windowHandle, m_instance->getHandle());

    auto suitableAdapter = selectSuitableAdapter(m_instance->queryAdapters(), m_surface.get());
    m_adapter = std::make_unique<SM::Adapter>(suitableAdapter);
    logAdapterSupportability(m_adapter.get(), m_surface.get());

    SM::DeviceOptions deviceOpt{};
    deviceOpt.requestedFeatures = { .dynamicRendering = true };
    
    std::vector<QueueRequest> queueRequests;

    // Device initialization
    //
    m_device = std::make_unique<SM::Device>(apiVersion, m_adapter.get(), deviceOpt);    

    // std::vector<QueueDescription> queueDescriptions = m_device.getQueues(queueRequests, queueFamilyProperties);

    // const uint32_t queueCount = queueDescriptions.size();
    // m_queues.reserve(queueCount);
    // for (uint32_t i = 0; i < queueCount; ++i) {
    //     m_queues.emplace_back(SM::Queue(queueDescriptions[i]));
    // }
}

SM::VulkanRHI::~VulkanRHI() {
    destroy();
}

void SM::VulkanRHI::destroy() {
    m_device->destroy();
    m_surface->destroy();
    m_instance->destroy();
}

SM::Adapter SM::VulkanRHI::selectSuitableAdapter(std::vector<SM::Adapter> adapters, const SM::Surface* pSurface) const {
    // Sorting adapters by deviceType:
    const std::vector<VkPhysicalDeviceType> deviceTypeOrders = {
        VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU, // best choise for us
        VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
        VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU, VK_PHYSICAL_DEVICE_TYPE_CPU,
        VK_PHYSICAL_DEVICE_TYPE_OTHER
    };

    std::vector<SM::Adapter> sortedAdapters;
    if (adapters.size() != 0) {
        sortedAdapters.reserve(adapters.size());
        for (auto type : deviceTypeOrders) {
            for (const auto& adapter : adapters) {
                if (adapter.properties().deviceType == type)
                    sortedAdapters.emplace_back(adapter);
            }
        }
    }
    

    auto isAdapterSuitable = [&](const SM::Adapter& adapter) -> bool {
        const auto queueFamilyProperties = adapter.queryQueueFamilyProperties();
        for (uint32_t i = 0; i < queueFamilyProperties.size(); ++i) {
            const auto& family = queueFamilyProperties[i];

            if (!family.supportsFeature(VK_QUEUE_GRAPHICS_BIT))
                return false;
            if (!family.supportsFeature(VK_QUEUE_COMPUTE_BIT))
                return false;
            if (!family.supportsFeature(VK_QUEUE_TRANSFER_BIT))
                return false;
            // Uncomment it when it need
            //         if(!family.supportsFeature(VK_QUEUE_SPARSE_BINDING_BIT)) return
            //         false;
            //         if(!family.supportsFeature(VK_QUEUE_VIDEO_DECODE_BIT_KHR))
            //         return false;
            // #if VK_ENABLE_BETA_EXTENSIONS
            //         if(!family.supportsFeature(VK_QUEUE_VIDEO_ENCODE_BIT_KHR))
            //         return false;
            // #endif
            //         if(!family.supportsFeature(VK_QUEUE_OPTICAL_FLOW_BIT_NV))
            //         return false;
            if (!family.supportsPresentation(adapter.getHandle(), pSurface->getHandle()))
                return false;
        }
        return true;
    };

    for (const auto& adapter : sortedAdapters) {
        if (!isAdapterSuitable(adapter)) {
            continue;
        }
        SM_LOG_DEBUG("RHI", "Selected adapter: {}", adapter.properties().deviceName);
        return adapter;
    }

    SM_LOG_CRITICAL("RHI", "Unable to find a suitable Adapter...");
    return {VK_NULL_HANDLE};
}
