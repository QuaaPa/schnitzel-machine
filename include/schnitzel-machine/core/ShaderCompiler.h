#ifndef SM_CORE_SHADERCOMPILER_H_
#define SM_CORE_SHADERCOMPILER_H_

#include <shaderc/shaderc.hpp>
#include <string>
#include <vector>
#include <optional>
#include <filesystem>

namespace SM {

    enum class ShaderStage {
        Vertex,
        Fragment,
        Compute,
        Geometry
    };

    struct ShaderCompileResult {
        std::vector<uint32_t> spirv;
        bool success = false;
        std::string errorMessage;
        // size_t numErrors = 0;
        // size_t numWarnings = 0;
    };

    class ShaderCompiler {
    public:
        ShaderCompileResult CompileFromSource(const std::string& source,
                                              ShaderStage stage,
                                              const std::string& debugName = "shader",
                                              const std::vector<std::pair<std::string, std::string>>& defines = {}) const;

        ShaderCompileResult CompileFromFile(const std::filesystem::path& path,
                                            std::optional<ShaderStage> stageOverride = std::nullopt) const;

        void SetOptimizationLevel(shaderc_optimization_level level) { m_optLevel = level; }
        void SetGenerateDebugInfo(bool enabled) { m_debugInfo = enabled; }
        void SetTargetVulkanVersion(uint32_t apiVersion); // VK_API_VERSION_1_2

    private:
        shaderc::Compiler m_compiler;
        
        shaderc_optimization_level m_optLevel = shaderc_optimization_level_performance;
        bool m_debugInfo = false;
        shaderc_env_version m_envVersion = shaderc_env_version_vulkan_1_2;

        static shaderc_shader_kind ToShadercKind(ShaderStage stage);
        static std::optional<ShaderStage> InferStageFromExtension(const std::filesystem::path& path);
    };

} // namespace SM

#endif // SM_CORE_SHADERCOMPILER_H_
