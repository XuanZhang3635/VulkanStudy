//
//  test_descriptor.hpp
//  VulkanProject
//
//  Created by zhang on 2024/1/6.
//

#pragma once

#include "test_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace test {

class TestDescriptorSetLayout {
 public:
  class Builder {
   public:
    Builder(TestDevice &testDevice) : testDevice{testDevice} {}

    Builder &addBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags,
        uint32_t count = 1);
    std::unique_ptr<TestDescriptorSetLayout> build() const;

   private:
    TestDevice &testDevice;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
  };

  TestDescriptorSetLayout(
      TestDevice &testDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
  ~TestDescriptorSetLayout();
  TestDescriptorSetLayout(const TestDescriptorSetLayout &) = delete;
  TestDescriptorSetLayout &operator=(const TestDescriptorSetLayout &) = delete;

  VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

 private:
  TestDevice &testDevice;
  VkDescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

  friend class TestDescriptorWriter;
};

class TestDescriptorPool {
 public:
  class Builder {
   public:
    Builder(TestDevice &testDevice) : testDevice{testDevice} {}

    Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
    Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
    Builder &setMaxSets(uint32_t count);
    std::unique_ptr<TestDescriptorPool> build() const;

   private:
    TestDevice &testDevice;
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;
  };

  TestDescriptorPool(
      TestDevice &testDevice,
      uint32_t maxSets,
      VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize> &poolSizes);
  ~TestDescriptorPool();
  TestDescriptorPool(const TestDescriptorPool &) = delete;
  TestDescriptorPool &operator=(const TestDescriptorPool &) = delete;

  bool allocateDescriptor(
      const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

  void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

  void resetPool();

 private:
  TestDevice &testDevice;
  VkDescriptorPool descriptorPool;

  friend class TestDescriptorWriter;
};

class TestDescriptorWriter {
 public:
  TestDescriptorWriter(TestDescriptorSetLayout &setLayout, TestDescriptorPool &pool);

  TestDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
  TestDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

  bool build(VkDescriptorSet &set);
  void overwrite(VkDescriptorSet &set);

 private:
  TestDescriptorSetLayout &setLayout;
  TestDescriptorPool &pool;
  std::vector<VkWriteDescriptorSet> writes;
};

}  // namespace test
