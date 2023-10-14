#pragma once

#include "lve_camera.hpp"
#include "lve_game_object.hpp"

// lib
DISABLE_WARNINGS_PUSH(
    4005 4201 4459 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)
#include <vulkan/vulkan.h>
DISABLE_WARNINGS_POP()

namespace lve {

    static inline constexpr auto MAX_LIGHTS = 10;

    struct PointLight {
        glm::vec4 position{};  // ignore w
        glm::vec4 color{};     // w is intensity
    };

    struct GlobalUbo {
        glm::mat4 projection{1.f};
        glm::mat4 view{1.f};
        glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .02f};  // w is intensity
        std::array<PointLight, MAX_LIGHTS> pointLights;
        int numLights;
    };

    struct FrameInfo {
        int frameIndex;
        long double frameTime;
        VkCommandBuffer commandBuffer;
        LveCamera &camera;
        VkDescriptorSet globalDescriptorSet;
        LveGameObject::Map &gameObjects;
    };
}  // namespace lve
