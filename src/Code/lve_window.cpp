#include "lve_window.hpp"

#ifdef _DEBUG
static constexpr long double TIME_DIVISOR = MICROSENCONDSFACTOR;
#else
static constexpr long double TIME_DIVISOR = MILLISENCONDSFACTOR;
#endif  // _DEBUG
namespace lve {
    DISABLE_WARNINGS_PUSH(26432)
    LveWindow::LveWindow(int ww, int hh, const std::string &name) : width{ww}, height{hh}, windowName{name} { initWindow(); }

    LveWindow::~LveWindow() noexcept {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    DISABLE_WARNINGS_POP()

    void LveWindow::initWindow() {
        Timer t;
        if(!glfwInit()) [[unlikely]] {
            LCRITICAL("Failed to initialize GLFW.");
            throw VKRAppError("Failed to initialize GLFW");
        }
        if(!glfwVulkanSupported()) {
            glfwTerminate();
            LCRITICAL("Failed to initialize GLFW. vulkan not supported");
            throw VKRAppError("Failed to initialize GLFW. Vulkan not supported");
        }
        glfwSetErrorCallback(errorCallback);
        t.stop();
        const auto glfwsetuptime = t.elapsedMilliseconds();
        Timer tt;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        tt.stop();
#ifdef _DEBUG
        static const auto glfwhintstime = tt.elapsedMicroseconds();
#else
        static const auto glfwhintstime = tt.elapsedNanoseconds();
#endif  // _DEBUG
        Timer ttt;
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        if(window == nullptr) [[unlikely]] {
            LCRITICAL("Failed to create GLFW window.");
            glfwTerminate();
            throw VKRAppError("Failed to create GLFW window");
        }
        glfwSetKeyCallback(window, keyCallback);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        ttt.stop();
        static const auto wcreationtime = ttt.elapsedMilliseconds();
        // Ottenimento delle dimensioni del monitor primario
        Timer tttt;
        GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
        if(primaryMonitor == nullptr) [[unlikely]] {
            LCRITICAL("Failed to get the primary monitor.");
            glfwDestroyWindow(window);
            glfwTerminate();
            throw VKRAppError("Failed to get primary monitor");
        }
        tttt.stop();
#ifdef _DEBUG
        static const auto primaryMonitortime = tttt.elapsedMicroseconds();
#else
        static const auto primaryMonitortime = tttt.elapsedNanoseconds();
#endif  // _DEBUG

        Timer ttttt;
        static const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
        if(mode == nullptr) [[unlikely]] {
            LCRITICAL("Failed to get the video mode of the primary monitor.");
            glfwDestroyWindow(window);
            glfwTerminate();
            throw VKRAppError("Failed to get video mode");
        }
        ttttt.stop();
        static const auto modetime = ttttt.elapsedMicroseconds();
        Timer tttttt;
        static const int monitorWidth = mode->width;
        static const int monitorHeight = mode->height;

        // Calcolo delle coordinate per centrare la finestra
        int windowWidth;
        int windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        static const int centerX = CALC_CENTRO(monitorWidth, windowWidth);
        static const int centerY = CALC_CENTRO(monitorHeight, windowHeight);
        tttttt.stop();
        static const auto repossetuptime = tttttt.elapsedMicroseconds();
        Timer ttttttt;

        // Posizionamento della finestra al centro del monitor
        glfwSetWindowPos(window, centerX, centerY);

        int posX = 0;
        int posY = 0;
        glfwGetWindowPos(window, &posX, &posY);
        if(posX != centerX || posY != centerY) [[unlikely]] {
            LCRITICAL("Failed to position the window at the center.");
            glfwDestroyWindow(window);
            glfwTerminate();
            throw VKRAppError("Failed to set window position centered");
        }
        ttttttt.stop();
        static const auto wrepostime = ttttttt.elapsedMicroseconds();

        static const auto totalwindowsetuptime = glfwsetuptime + (glfwhintstime / TIME_DIVISOR) + wcreationtime;
        static const auto reposition = primaryMonitortime + modetime + repossetuptime + wrepostime;
        static const auto totalsetuptime = totalwindowsetuptime + (reposition / C_LD(1000.0));
        glfwShowWindow(window);

        LINFO("setup time for glfw = {:f} ms", glfwsetuptime);
        INFO_LOG_TIME("Time spent on GLFW Hints Setup", glfwhintstime);
        LINFO("time for creating the window {0}: (w: {1}, h: {2}, pos:(x:{3}, y:{4})) = {5:f} ms", windowTitle.data(), w, h,
              centerX, centerY, wcreationtime);
        INFO_LOG_TIME("Time taken to retrieve the primary monitor", primaryMonitortime);
        LINFO("Time taken to retrieve the primary monitor mode = {:f} us", modetime);
        LINFO("Time taken for the setup of window repositioning = {:f} us", repossetuptime);
        LINFO("time for repositionig the window{0} = {1:f} us", windowTitle.data(), wrepostime);
        LINFO("toltal setup time = {:f} ms", totalsetuptime);
    }

    void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        VK_CHECK(glfwCreateWindowSurface(instance, window, nullptr, surface), VKRAppError("failed to create window surface!"));
    }
    void LveWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) noexcept {
        auto lveWindow = static_cast<LveWindow *>(glfwGetWindowUserPointer(window));
        lveWindow->framebufferResized = true;
        lveWindow->width = width;
        lveWindow->height = height;
    }
}  // namespace lve
