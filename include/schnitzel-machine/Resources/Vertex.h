#ifndef SM_RESOURCES_VERTEX_H_
#define SM_RESOURCES_VERTEX_H_

#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>

namespace SM {
    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription() {
            return VkVertexInputBindingDescription{
                .binding = 0,
                .stride = sizeof(Vertex),
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
            };
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            return {
                VkVertexInputAttributeDescription{ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = offsetof(Vertex, pos) },
                VkVertexInputAttributeDescription{ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(Vertex, color) }
            };
        }
    };
}; // namespace SM

#endif // SM_RESOURCES_VERTEX_H_
