//
//  frist_app.cpp
//  VulkanProject
//
//  Created by zhang on 2023/12/20.
//

#include "frist_app.hpp"
#include <array>

namespace test {

FirstApp::FirstApp() {
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
    createCommanBuffers();
}

/*
void sierpinski(
    std::vector<TestModel::Vertex> &vertices,
    int depth,
    glm::vec2 left,
    glm::vec2 right,
    glm::vec2 top) {
  if (depth <= 0) {
    vertices.push_back({top});
    vertices.push_back({right});
    vertices.push_back({left});
  } else {
    auto leftTop = 0.5f * (left + top);
    auto rightTop = 0.5f * (right + top);
    auto leftRight = 0.5f * (left + right);
    sierpinski(vertices, depth - 1, left, leftRight, leftTop);
    sierpinski(vertices, depth - 1, leftRight, right, rightTop);
    sierpinski(vertices, depth - 1, leftTop, rightTop, top);
  }
}*/

void FirstApp::loadModels(){
    std::vector<TestModel::Vertex> vertices{
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
    
    //std::vector<TestModel::Vertex> vertices{};
    //sierpinski(vertices, 5, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
    testModel = std::make_unique<TestModel>(testDevice, vertices);
}

FirstApp::~FirstApp() {
    vkDestroyPipelineLayout(testDevice.device(), pipelineLayout, nullptr);
    
}

void FirstApp::run(){
    while (!testWindow.shouldClose()) {
        glfwPollEvents();
        drawFrame();
    }
    
    vkDeviceWaitIdle(testDevice.device());
}

void FirstApp::createPipelineLayout(){
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if(vkCreatePipelineLayout(testDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void FirstApp::createPipeline() {
    
    
    PipelineConfigInfo pipelineConfig{};
    TestPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = testSwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    testPipeline = std::make_unique<TestPipeline>(testDevice,
                                                  "shaders/simple_shader.vert.spv",
                                                  "shaders/simple_shader.frag.spv",
                                                  pipelineConfig);
}

void FirstApp::createCommanBuffers(){
    commanBuffers.resize(testSwapChain->imageCount());
    
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = testDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commanBuffers.size());
    
    if(vkAllocateCommandBuffers(testDevice.device(), &allocInfo, commanBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
    
}


void FirstApp::freeCommandBuffers() {
    vkFreeCommandBuffers(testDevice.device(), testDevice.getCommandPool(), static_cast<uint32_t>(commanBuffers.size()), commanBuffers.data());
    commanBuffers.clear();
}

void FirstApp::recreateSwapChain() {
    auto extent = testWindow.getExtent();
    while(extent.width == 0 || extent.height == 0) {
        extent = testWindow.getExtent();
        glfwWaitEvents();
    }
    
    vkDeviceWaitIdle(testDevice.device());
    
    if(testSwapChain == nullptr) {
        testSwapChain = std::make_unique<TestSwapChain>(testDevice, extent);
    } else {
        // move function: 
        testSwapChain = std::make_unique<TestSwapChain>(testDevice, extent, std::move(testSwapChain));
        if(testSwapChain->imageCount() != commanBuffers.size()) {
            freeCommandBuffers();
            createCommanBuffers();
        }
    }
    
    createPipeline();
}

void FirstApp::recordCommandBuffer(int imageIndex){
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    
    if(vkBeginCommandBuffer(commanBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = testSwapChain->getRenderPass();
    renderPassInfo.framebuffer = testSwapChain->getFrameBuffer(imageIndex);//swap chain not window
    
    renderPassInfo.renderArea.offset = {0,0};
    renderPassInfo.renderArea.extent = testSwapChain->getSwapChainExtent();
    
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f,0.1f,0.1f,1.0f};
    clearValues[1].depthStencil = {1, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
    
    vkCmdBeginRenderPass(commanBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(testSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(testSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0,0}, testSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commanBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commanBuffers[imageIndex], 0, 1, &scissor);
    
    
    testPipeline->bind(commanBuffers[imageIndex]);
    // vkCmdDraw(commanBuffers[i], 3, 1, 0, 0);
    testModel->bind(commanBuffers[imageIndex]);
    testModel->draw(commanBuffers[imageIndex]);
    
    vkCmdEndRenderPass(commanBuffers[imageIndex]);
    if(vkEndCommandBuffer(commanBuffers[imageIndex]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void FirstApp::drawFrame(){
    
    uint32_t imageIndex;
    auto result = testSwapChain->acquireNextImage(&imageIndex);
    
    if(result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }
    
    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
    
    recordCommandBuffer(imageIndex);
    result = testSwapChain->submitCommandBuffers(&commanBuffers[imageIndex], &imageIndex);
    
    if(result == VK_ERROR_OUT_OF_DATE_KHR || result==VK_SUBOPTIMAL_KHR ||
       testWindow.wasWindowResized()) {
        testWindow.resetWindowResizedFlag();
        recreateSwapChain();
        return;
    }
    
    if(result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }
}


}
