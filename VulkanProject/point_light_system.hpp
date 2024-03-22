//
//  point_light_system.hpp
//  VulkanProject
//
//  Created by zhang on 2024/1/6.
//

#pragma once

#include "test_camera.hpp"
#include "test_device.hpp"
#include "test_frame_info.hpp"
#include "test_game_object.hpp"
#include "test_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace test {
class PointLightSystem {
 public:
  PointLightSystem(
      TestDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~PointLightSystem();

  PointLightSystem(const PointLightSystem &) = delete;
  PointLightSystem &operator=(const PointLightSystem &) = delete;

  void render(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  TestDevice &testDevice;

  std::unique_ptr<TestPipeline> testPipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace test
