#pragma once

#include "lve_device.hpp"
#include "lve_swap_chain.hpp"
#include "lve_window.hpp"

namespace lve {
    class LveRenderer {
    public:
        LveRenderer(LveWindow &window, LveDevice &device);
        ~LveRenderer();

        LveRenderer(const LveRenderer &) = delete;
        LveRenderer &operator=(const LveRenderer &) = delete;

        [[nodiscard]] inline VkRenderPass getSwapChainRenderPass() const noexcept { return lveSwapChain->getRenderPass(); }
        [[nodiscard]] inline float getAspectRatio() const noexcept { return lveSwapChain->extentAspectRatio(); }
        [[nodiscard]] inline bool isFrameInProgress() const noexcept { return isFrameStarted; }

        [[nodiscard]] inline VkCommandBuffer getCurrentCommandBuffer() const noexcept {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers.at(currentFrameIndex);
        }

        [[nodiscard]] inline int getFrameIndex() const noexcept {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer) noexcept;
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer) const noexcept;

    private:
        void createCommandBuffers();
        void freeCommandBuffers() noexcept;
        void recreateSwapChain();

        LveWindow &lveWindow;
        LveDevice &lveDevice;
        std::unique_ptr<LveSwapChain> lveSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted{false};
    };
}  // namespace lve
