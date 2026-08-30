#ifndef SM_CORE_CORE_H_
#define SM_CORE_CORE_H_

#include <filesystem>
#include <memory>

#include <vulkan/vulkan.h>

#include "RHI/VulkanRHI.h"
#include "core/Window.h"
#include "core/ShaderCompiler.h"

namespace SM {
    class Engine {
    private:        
        Engine()  = default;
        ~Engine() = default;

        Window* win;
        std::unique_ptr<SM::VulkanRHI> rhi;
        std::unique_ptr<SM::ShaderCompiler> compiler;
        
    public:
        static Engine* getInstance() {
            static Engine instance;
            return &instance;
        }

        void run(int argc, char *argv[]);
    
    private:
        void init(std::filesystem::path exeDir);
    
        void mainLoop();
        void cleanup();

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
    };    
}

#endif // SM_CORE_CORE_H_
