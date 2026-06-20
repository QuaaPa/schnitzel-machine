#ifndef PIPELINE_BUILDER_HH
#define PIPELINE_BUILDER_HH

#include <vulkan/vulkan_core.h>
#include "core/Vulkan/VulkanPipeline.hh"

struct PipelineBuilder {
    VulkanPipeline build();
};

#endif // PIPELINE_BUILDER_HH
