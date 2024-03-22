//
//  test_model.hpp
//  VulkanProject
//
//  Created by zhang on 2023/12/27.
//

# pragma once

#include "test_device.hpp"
#include "test_buffer.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>
#include <memory>

namespace test {
class TestModel{
public:
    struct Vertex{
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec3 normal{};
        glm::vec2 uv{};
        
        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        
        bool operator==(const Vertex &other) const {
            return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
        }
        
    };
    
    //TestModel(TestDevice &device, const std::vector<Vertex> &vertices);
    struct Builder {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};
        
        void loadModel(const std::string &filepath);
      };

    TestModel(TestDevice &device, const TestModel::Builder &builder);
    ~TestModel();
    
    TestModel(const TestModel &) = delete;
    TestModel &operator=(const TestModel &) = delete;
    
    static std::unique_ptr<TestModel> createModelFromFile(TestDevice &device, const std::string &filepath);
    
    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);
    
private:
    void createVertexBuffers(const std::vector<Vertex> &vertices);
    void createIndexBuffers(const std::vector<uint32_t> &indices);
    
    TestDevice &testDevice;
    //VkBuffer vertexBuffer;
    //VkDeviceMemory vertexBufferMemory;
    std::unique_ptr<TestBuffer> vertexBuffer;
    uint32_t vertexCount;
    
    bool hasIndexBuffer = false;
    //VkBuffer indexBuffer;
    //VkDeviceMemory indexBufferMemory;
    std::unique_ptr<TestBuffer> indexBuffer;
    uint32_t indexCount;
};

}
