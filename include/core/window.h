#ifndef SM_CORE_WINDOW_H_
#define SM_CORE_WINDOW_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace sm {
    class Window {
    private:
        Window() {};

        GLFWwindow *m_pwindow;

        int m_width;
        int m_height;

    public:
        void init(int width, int height, const char* title);

        static Window* getInstance() {
            static Window instance;
            return &instance;
        }

        GLFWwindow* getGlfwWindow() const {return m_pwindow;};
        
        bool shouldClose();
        void pollEvents();
        void drawFrame();
        void destroy();
    };
}
#endif // SM_CORE_WINDOW_H_
