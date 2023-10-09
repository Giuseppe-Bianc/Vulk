#pragma once

#include "lve_device.hpp"

namespace lve {

    class LveBuffer {
    public:
        LveBuffer(LveDevice &device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags,
                  VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);
        ~LveBuffer();

        LveBuffer(const LveBuffer &) = delete;
        LveBuffer &operator=(const LveBuffer &) = delete;

        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) noexcept;
        void unmap() noexcept;

        void writeToBuffer(void *data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) noexcept;
        VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) noexcept;
        VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) noexcept;
        VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) noexcept;

        void writeToIndex(void *data, int index) noexcept;
        VkResult flushIndex(int index) noexcept;
        VkDescriptorBufferInfo descriptorInfoForIndex(int index) noexcept;
        VkResult invalidateIndex(int index) noexcept;

        [[nodiscard]] inline VkBuffer getBuffer() const noexcept { return buffer; }
        [[nodiscard]] inline void *getMappedMemory() const noexcept { return mapped; }
        [[nodiscard]] inline uint32_t getInstanceCount() const noexcept { return instanceCount; }
        [[nodiscard]] inline VkDeviceSize getInstanceSize() const noexcept { return instanceSize; }
        [[nodiscard]] inline VkDeviceSize getAlignmentSize() const noexcept { return instanceSize; }
        [[nodiscard]] inline VkBufferUsageFlags getUsageFlags() const noexcept { return usageFlags; }
        [[nodiscard]] inline VkMemoryPropertyFlags getMemoryPropertyFlags() const noexcept { return memoryPropertyFlags; }
        [[nodiscard]] inline VkDeviceSize getBufferSize() const noexcept { return bufferSize; }

    private:
        static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) noexcept;

        LveDevice &lveDevice;
        void *mapped = nullptr;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;

        VkDeviceSize bufferSize;
        uint32_t instanceCount;
        VkDeviceSize instanceSize;
        VkDeviceSize alignmentSize;
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags memoryPropertyFlags;
    };

}  // namespace lve
