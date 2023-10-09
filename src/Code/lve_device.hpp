#pragma once

#include "lve_window.hpp"

namespace lve {

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats{};
        std::vector<VkPresentModeKHR> presentModes{};
        static void printDetails(const SwapChainSupportDetails &swapCainsDetails) noexcept;
    };

    struct QueueFamilyIndices {
        uint32_t graphicsFamily{};
        uint32_t presentFamily{};
        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;
#pragma optimize("gt", on)
        [[nodiscard]] inline bool isComplete() const noexcept { return graphicsFamilyHasValue && presentFamilyHasValue; }
    };

    class LveDevice {
    public:
#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif

        explicit LveDevice(LveWindow &window);
        ~LveDevice();

        // Not copyable or movable
        LveDevice(const LveDevice &) = delete;
        LveDevice &operator=(const LveDevice &) = delete;
        LveDevice(LveDevice &&) = delete;
        LveDevice &operator=(LveDevice &&) = delete;

        [[nodiscard]] inline VkCommandPool getCommandPool() noexcept { return commandPool; }
        [[nodiscard]] inline VkDevice device() noexcept { return device_; }
        [[nodiscard]] inline VkSurfaceKHR surface() noexcept { return surface_; }
        [[nodiscard]] inline VkQueue graphicsQueue() noexcept { return graphicsQueue_; }
        [[nodiscard]] inline VkQueue presentQueue() noexcept { return presentQueue_; }

        [[nodiscard]] inline SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags prop);
        [[nodiscard]] inline QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                     VkFormatFeatureFlags features);

        // Buffer Helper Functions
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags prop, VkBuffer &buffer,
                          VkDeviceMemory &bufferMemory);
        [[nodiscard]] VkCommandBuffer beginSingleTimeCommands() noexcept;
        void endSingleTimeCommands(VkCommandBuffer commandBuffer) noexcept;
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) noexcept;
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) noexcept;
        void createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags prop, VkImage &image,
                                 VkDeviceMemory &imageMemory);

        VkPhysicalDeviceProperties properties;

    private:
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createCommandPool();

        // helper functions
        bool isDeviceSuitable(VkPhysicalDevice device);
        std::vector<const char *> getRequiredExtensions();
        bool checkValidationLayerSupport();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) const noexcept;
        void hasGflwRequiredInstanceExtensions();
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

        VkInstance instance{};
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        LveWindow &window;
        VkCommandPool commandPool{};

        VkDevice device_{};
        VkSurfaceKHR surface_{};
        VkQueue graphicsQueue_{};
        VkQueue presentQueue_{};

        static inline const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
        static inline const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    };

}  // namespace lve
