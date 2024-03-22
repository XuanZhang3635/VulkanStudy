//
//  test_frame_info.hpp
//  VulkanProject
//
//  Created by zhang on 2024/1/5.
//

#pragma once

#include "test_camera.hpp"
#include "test_game_object.hpp"

// lib
#include <vulkan/vulkan.h>

namespace test {

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  TestCamera &camera;
  VkDescriptorSet globalDescriptorSet;
  TestGameObject::Map &gameObjects;
};

}
