//
//  test_model.hpp
//  VulkanProject
//
//  Created by zhang on 2023/12/27.
//

# pragma once

#include "test_device.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
// std
#include <vector>
namespace test {
class TestModel{
public:
    struct Vertex{
        glm::vec2 position;
        glm::vec3 color;
        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };
    
    TestModel(TestDevice &device, const std::vector<Vertex> &vertices);
    ~TestModel();
    
    TestModel(const TestModel &) = delete;
    TestModel &operator=(const TestModel &) = delete;
    
    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);
    
private:
    void createVertexBuffers(const std::vector<Vertex> &vertices);
    
    TestDevice &testDevice;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;
};

}
