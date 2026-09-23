#ifndef SM_RESOURCES_RENDERER_H_
#define SM_RESOURCES_RENDERER_H_

#include <cstdint>

namespace SM {
    class Renderer {
    public:
        Renderer();

        void beginFrame();
        void drawExample();
        void endFrame();
        
    private:        
        uint32_t currentFrame;
    };
    
}; // namespace SM

#endif // SM_RESOURCES_RENDERER_H_
