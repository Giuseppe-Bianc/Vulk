#pragma once

#include "lve_device.hpp"

namespace lve {

    class LveSwapChain {
    public:
        static constexpr inline int MAX_FRAMES_IN_FLIGHT = 2;

        LveSwapChain(LveDevice &deviceRef, VkExtent2D windowExtent);
        LveSwapChain(LveDevice &deviceRef, VkExtent2D windowExtent, std::shared_ptr<LveSwapChain> previous);

        ~LveSwapChain();

        LveSwapChain(const LveSwapChain &) = delete;
        LveSwapChain &operator=(const LveSwapChain &) = delete;

#pragma optimize("gt", on)
        [[nodiscard]] inline VkFramebuffer getFrameBuffer(int index) noexcept { return swapChainFramebuffers.at(index); }
#pragma optimize("gt", on)
        [[nodiscard]] inline VkRenderPass getRenderPass() noexcept { return renderPass; }
#pragma optimize("gt", on)
        [[nodiscard]] inline VkImageView getImageView(int index) noexcept { return swapChainImageViews.at(index); }
#pragma optimize("gt", on)
        [[nodiscard]] inline size_t imageCount() const noexcept { return swapChainImages.size(); }
#pragma optimize("gt", on)
        [[nodiscard]] inline VkFormat getSwapChainImageFormat() const noexcept { return swapChainImageFormat; }
#pragma optimize("gt", on)
        [[nodiscard]] inline VkExtent2D getSwapChainExtent() const noexcept { return swapChainExtent; }
#pragma optimize("gt", on)
        [[nodiscard]] inline uint32_t width() const noexcept { return swapChainExtent.width; }
#pragma optimize("gt", on)
        [[nodiscard]] inline uint32_t height() const noexcept { return swapChainExtent.height; }
#pragma optimize("gt", on)
        [[nodiscard]] inline float extentAspectRatio() const noexcept {
            return C_F(swapChainExtent.width) / C_F(swapChainExtent.height);
        }
        VkFormat findDepthFormat();

        VkResult acquireNextImage(uint32_t *imageIndex) noexcept;
        VkResult submitCommandBuffers(const VkCommandBuffer *buffers, const uint32_t *imageIndex);

#pragma optimize("gt", on)
        [[nodiscard]] inline bool compareSwapFormats(const LveSwapChain &swapChaine) const noexcept {
            return swapChaine.swapChainDepthFormat == swapChainDepthFormat &&
                   swapChaine.swapChainImageFormat == swapChainImageFormat;
        }

    private:
        void init();
        void createSwapChain();
        void createImageViews();
        void createDepthResources();
        void createRenderPass();
        void createFramebuffers();
        void createSyncObjects();

        // Helper functions
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) const noexcept;
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) const;
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const noexcept;

        VkFormat swapChainImageFormat;
        VkFormat swapChainDepthFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkRenderPass renderPass;

        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImageMemorys;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        LveDevice &device;
        VkExtent2D windowExtent;

        VkSwapchainKHR swapChain;
        std::shared_ptr<LveSwapChain> oldSwapChain;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame = 0;
    };

}  // namespace lve
