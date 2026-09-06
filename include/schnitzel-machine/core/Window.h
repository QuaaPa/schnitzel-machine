#ifndef SM_CORE_WINDOW_H_
#define SM_CORE_WINDOW_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace SM {
    template<typename T>
    struct FramebufferSize {
        const T width;
        const T height;
    };

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

        template<typename T = int>
        SM::FramebufferSize<T> getFramebufferSize() {
            int w, h;
            glfwGetFramebufferSize(m_pwindow, &w, &h);
            return FramebufferSize<T> {
                .width = static_cast<T>(w),
                .height = static_cast<T>(h)
            };
        }
        
        bool shouldClose();
        void pollEvents();
        void drawFrame();
        void destroy();
    };
} // namespace SM
#endif // SM_CORE_WINDOW_H_
