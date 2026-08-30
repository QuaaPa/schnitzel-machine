#include "core/ShaderCompiler.h"

#include <fstream>
#include <sstream>

shaderc_shader_kind SM::ShaderCompiler::ToShadercKind(SM::ShaderStage stage) {
    switch (stage) {
    case SM::ShaderStage::Vertex:       return shaderc_vertex_shader;
    case SM::ShaderStage::Fragment:     return shaderc_fragment_shader;
    case SM::ShaderStage::Compute:      return shaderc_compute_shader;
    case SM::ShaderStage::Geometry:     return shaderc_geometry_shader;
    }
    return shaderc_glsl_infer_from_source;
}

std::optional<SM::ShaderStage> SM::ShaderCompiler::InferStageFromExtension(const std::filesystem::path& path) {
    auto ext = path.extension().string();
    if (ext == ".vert") return ShaderStage::Vertex;
    if (ext == ".frag") return ShaderStage::Fragment;
    if (ext == ".comp") return ShaderStage::Compute;
    if (ext == ".geom") return ShaderStage::Geometry;
    return std::nullopt;
}

SM::ShaderCompileResult SM::ShaderCompiler::CompileFromSource(const std::string& source,
                                                              ShaderStage stage,
                                                              const std::string& debugName,
                                                              const std::vector<std::pair<std::string, std::string>>& defines) const
{
    // setting up options for each compiling shader
    shaderc::CompileOptions options;
    options.SetOptimizationLevel(m_optLevel);
    options.SetTargetEnvironment(shaderc_target_env_vulkan, m_envVersion);

    if (m_debugInfo) {
        options.SetGenerateDebugInfo();
    }

    for (const auto& [name, value] : defines) {
        options.AddMacroDefinition(name, value);
    }

    shaderc::SpvCompilationResult result = m_compiler.CompileGlslToSpv(source,
                                                                       ToShadercKind(stage),
                                                                       debugName.c_str(),
                                                                       options);

    ShaderCompileResult out;
    // out.errorMessage = result.GetNumErrors();
    // out.numWarnings = result.GetNumWarnings();
    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        out.success = false;
        out.errorMessage = result.GetErrorMessage();
        return out;
    }

    out.success = true;
    out.spirv = { result.cbegin(), result.cend() };
    return out;
}

SM::ShaderCompileResult SM::ShaderCompiler::CompileFromFile(const std::filesystem::path& path,
                                                            std::optional<ShaderStage> stageOverride) const
{
    std::ifstream file(path);
    if (!file.is_open()) {
        return SM::ShaderCompileResult{
            .success = false,
            .errorMessage = "Failed to open file: " + path.string()
        };
    }

    std::stringstream ss;
    ss << file.rdbuf();

    SM::ShaderStage stage;
    if (stageOverride) {
        stage = *stageOverride;
    } else if (auto inferred = InferStageFromExtension(path)) {
        stage = *inferred;
    } else {
        return SM::ShaderCompileResult{
            .success = false,
            .errorMessage = "Cannot infer shader stage from extension: " + path.string(),
        };
    }

    return CompileFromSource(ss.str(), stage, path.filename().string());
}
