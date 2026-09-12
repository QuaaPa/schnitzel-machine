#include "Resources/CommandPool.h"

#include <vulkan/vulkan_core.h>

#include "RHI/VkResultToString.h"

SM::CommandPool::~CommandPool() {
    if (m_commandPool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);
        m_commandPool = VK_NULL_HANDLE;
    }
    m_device = VK_NULL_HANDLE;
}

SM::CommandPool::CommandPool(SM::CommandPool&& other) noexcept {
    m_commandPool = other.m_commandPool;
    m_device = other.m_device;
    other.m_commandPool = VK_NULL_HANDLE;
    other.m_device = VK_NULL_HANDLE;
}

SM::CommandPool& SM::CommandPool::operator=(SM::CommandPool&& other) noexcept {
    if (this != &other) {
        if (m_commandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(m_device, m_commandPool, nullptr);
            m_commandPool = VK_NULL_HANDLE;
        }
        m_commandPool = other.m_commandPool;
        m_device = other.m_device;
        other.m_commandPool = VK_NULL_HANDLE;
        other.m_device = VK_NULL_HANDLE;
    }
    return *this;
}

SM::Result SM::CommandPool::initializeCommandPool(VkDevice vkDevice, const uint32_t queueFamilyIndex) {

    m_device = vkDevice;
    
    VkCommandPoolCreateInfo vkCommandPoolInfo{};
    vkCommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    vkCommandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    vkCommandPoolInfo.queueFamilyIndex = queueFamilyIndex;

    return vkCreateCommandPool(m_device, &vkCommandPoolInfo, nullptr, &m_commandPool);
}
