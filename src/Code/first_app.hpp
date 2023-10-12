#pragma once

#include "lve_descriptors.hpp"
#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_renderer.hpp"
#include "lve_window.hpp"

namespace lve {
    static inline constexpr std::string_view titleBase = "Vulkan Tutorial. ";
    class FirstApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        DISABLE_WARNINGS_PUSH(26455)
        FirstApp();
        ~FirstApp() noexcept = default;

        FirstApp(const FirstApp &) = delete;
        FirstApp &operator=(const FirstApp &) = delete;
        FirstApp(FirstApp &&) = delete;
        FirstApp &operator=(FirstApp &&) = delete;
        DISABLE_WARNINGS_POP()

        void run();

    private:
        void loadGameObjects();
        void updateFrameRate(const long double &frametime);

        LveWindow lveWindow{w, h, titleBase.data()};
        LveDevice lveDevice{lveWindow};
        LveRenderer lveRenderer{lveWindow, lveDevice};

        // note: order of declarations matters
        std::unique_ptr<LveDescriptorPool> globalPool{};
        LveGameObject::Map gameObjects;
        int frameCount{};
        long double totalTime{};
    };
}  // namespace lve
