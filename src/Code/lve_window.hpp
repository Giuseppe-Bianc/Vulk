#pragma once
#include "Timer.h"
#include "VKRAppError.h"
#include "headers.h"

#pragma optimize("gt", on)
static inline void errorCallback(int error, const char *description) { GLWFERR(error, description) }

#pragma optimize("gt", on)
static inline void keyCallback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action,
                               [[maybe_unused]] int mods) {
    switch(key) {
    case GLFW_KEY_ESCAPE:
        if(action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            LINFO("escape close");
        }
        break;
    // Add more cases for other keys if needed
    [[likely]] default:
        // Handle other keys here
        break;
    }
}

namespace lve {

    class LveWindow {
    public:
        LveWindow() noexcept = default;
        LveWindow(int ww, int hh, const std::string &name);
        ~LveWindow() noexcept;

        LveWindow(const LveWindow &) = delete;
        LveWindow &operator=(const LveWindow &) = delete;
#pragma optimize("gt", on)
        [[nodiscard]] bool shouldClose() noexcept { return NC_BOOL(glfwWindowShouldClose(window)); }
#pragma optimize("gt", on)
        [[nodiscard]] inline VkExtent2D getExtent() const noexcept { return {C_UI32T(width), C_UI32T(height)}; }
#pragma optimize("gt", on)
        [[nodiscard]] inline bool wasWindowResized() const noexcept { return framebufferResized; }
#pragma optimize("gt", on)
        [[nodiscard]] inline GLFWwindow *getGLFWwindow() const noexcept { return window; }
        void resetWindowResizedFlag() noexcept { framebufferResized = false; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
        void setTitle(const char *title) noexcept { glfwSetWindowTitle(window, title); }

    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height) noexcept;
        void initWindow();

        int width{};
        int height{};
        bool framebufferResized = false;

        std::string windowName;
        GLFWwindow *window = nullptr;
    };
}  // namespace lve
