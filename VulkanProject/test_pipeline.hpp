//
//  test_pipeline.hpp
//  VulkanProject
//
//  Created by zhang on 2023/12/20.
//

#pragma once

#include "test_device.hpp"
#include <string>
#include <vector>
#include <cstdint>
namespace test {

struct PipelineConfigInfo {
    PipelineConfigInfo() = default;
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
    
    // not needed when using dynamic
    //VkViewport viewport;
    //VkRect2D scissor;
    
    std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    // not default
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class TestPipeline {
public:
    TestPipeline(
                 TestDevice &device,
                 const std::string& vertFilepath,
                 const std::string& fragFilepath,
                 const PipelineConfigInfo& configInfo);
    ~TestPipeline();
    
    TestPipeline(const TestPipeline&) = delete;
    TestPipeline &operator=(const TestPipeline&)=delete;
    
    void bind(VkCommandBuffer commandBuffer);
    
    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
    
private:
    static std::vector<char> readFile(const std::string& filepath);
    
    void createGraphicsPipeline(
                                const std::string& vertFilepath,
                                const std::string& fragFilepath,
                                const PipelineConfigInfo& configInfo);
    
    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
    
    TestDevice& testDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};

}
