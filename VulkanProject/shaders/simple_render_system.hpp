//
//  simple_render_system.hpp
//  VulkanProject
//
//  Created by zhang on 2023/12/31.
//
#pragma once

#include "test_camera.hpp"
#include "test_device.hpp"
#include "test_game_object.hpp"
#include "test_pipeline.hpp"
#include "test_frame_info.hpp"

// std
#include <memory>
#include <vector>

namespace test {

class SimpleRenderSystem {
    
public:
    SimpleRenderSystem(TestDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    ~SimpleRenderSystem();
    
    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;
    
    void renderGameObjects(FrameInfo &frameInfo);
    
private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);
    
    TestDevice &testDevice;
    
    std::unique_ptr<TestPipeline> testPipeline;
    VkPipelineLayout pipelineLayout;
    
};

}
