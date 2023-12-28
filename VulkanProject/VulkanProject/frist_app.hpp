//
//  frist_app.hpp
//  VulkanProject
//
//  Created by zhang on 2023/12/20.
//

#ifndef frist_app_hpp
#define frist_app_hpp

#include <stdio.h>

#pragma one
#include "test_window.h"
#include "test_pipeline.hpp"
#include "test_device.hpp"
#include "test_swap_chain.hpp"
#include "test_model.hpp"

// std
#include <memory>

namespace test {
class FirstApp {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HIEGHT = 600;
    
    FirstApp();
    ~FirstApp();
    
    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;
    
    
    void run();
    
private:
    void loadModels();
    void createPipelineLayout();
    void createPipeline();
    void createCommanBuffers();
    void freeCommandBuffers();
    void drawFrame();
    void recreateSwapChain();
    void recordCommandBuffer(int imageIndex);
    
    TestWindow testWindow{WIDTH, HIEGHT, "Hello Vulkan!"};
    TestDevice testDevice{testWindow};
    std::unique_ptr<TestSwapChain> testSwapChain;
    //TestPipeline testPipeline{testDevice, "shaders/simple_shader.vert.spv","shaders/simple_shader.frag.spv",
     //   TestPipeline::defaultPipelineConfigInfo(WIDTH, HIEGHT)
    //};
    std::unique_ptr<TestPipeline> testPipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commanBuffers;
    std::unique_ptr<TestModel> testModel;
};
}

#endif /* frist_app_hpp */
