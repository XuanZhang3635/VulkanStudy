//
//  test_renderer.hpp
//  VulkanProject
//
//  Created by zhang on 2024/1/2.
//

#pragma once

#include "test_device.hpp"
#include "test_swap_chain.hpp"
#include "test_window.h"

// std
namespace test {

class TestRenderer {
    
public:
    TestRenderer(TestWindow &window, TestDevice &device);
    ~TestRenderer();
    
    TestRenderer(const TestRenderer &) = delete;
    TestRenderer &operator=(const TestRenderer &) = delete;
    
    VkRenderPass getSwapChainRenderPass() const { return testSwapChain->getRenderPass(); }
    
    float getAspectRatio() const { return testSwapChain->extentAspectRatio(); }
    
    bool isFrameInProgress() const { return isFrameStarted;}
    
    VkCommandBuffer getCurrentCommandBuffer() const {
        //assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentFrameIndex];
    }
    
    int getFrameIndex() const {
        //assert(isFrameStarted && "Cannot get frame index when frame not in progress");
        return currentFrameIndex;
    }
    
    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
    
private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();
    
    TestWindow &testWindow;
    TestDevice &testDevice;
    std::unique_ptr<TestSwapChain> testSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;
    
    uint32_t currentImageIndex;
    int currentFrameIndex;
    bool isFrameStarted;
    
};

}

