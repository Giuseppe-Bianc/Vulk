#pragma once

#include "lve_device.hpp"

namespace lve {

    class LveDescriptorSetLayout {
    public:
        class Builder {
        public:
            explicit Builder(LveDevice &lveDevice) : lveDevice{lveDevice} {}

            Builder &addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags,
                                uint32_t count = 1);
            std::unique_ptr<LveDescriptorSetLayout> build() const;

        private:
            LveDevice &lveDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        LveDescriptorSetLayout(LveDevice &lveDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~LveDescriptorSetLayout();
        LveDescriptorSetLayout(const LveDescriptorSetLayout &) = delete;
        LveDescriptorSetLayout &operator=(const LveDescriptorSetLayout &) = delete;

        [[nodiscard]] inline VkDescriptorSetLayout getDescriptorSetLayout() const noexcept { return descriptorSetLayout; }

    private:
        LveDevice &lveDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class LveDescriptorWriter;
    };

    class LveDescriptorPool {
    public:
        class Builder {
        public:
            explicit Builder(LveDevice &lveDevice) noexcept : lveDevice{lveDevice} {}

            Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags) noexcept;
            Builder &setMaxSets(uint32_t count) noexcept;
            std::unique_ptr<LveDescriptorPool> build() const;

        private:
            LveDevice &lveDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        LveDescriptorPool(LveDevice &lveDevice, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags,
                          const std::vector<VkDescriptorPoolSize> &poolSizes);
        ~LveDescriptorPool();
        LveDescriptorPool(const LveDescriptorPool &) = delete;
        LveDescriptorPool &operator=(const LveDescriptorPool &) = delete;

        bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const noexcept;

        void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const noexcept;

        void resetPool() noexcept;

    private:
        LveDevice &lveDevice;
        VkDescriptorPool descriptorPool;

        friend class LveDescriptorWriter;
    };

    class LveDescriptorWriter {
    public:
        LveDescriptorWriter(LveDescriptorSetLayout &setLayout, LveDescriptorPool &pool) noexcept;

        LveDescriptorWriter &writeBuffer(uint32_t binding, const VkDescriptorBufferInfo *bufferInfo);
        LveDescriptorWriter &writeImage(uint32_t binding, const VkDescriptorImageInfo *imageInfo);

        bool build(VkDescriptorSet &set) noexcept;
        void overwrite(VkDescriptorSet &set) noexcept;

    private:
        LveDescriptorSetLayout &setLayout;
        LveDescriptorPool &pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}  // namespace lve
