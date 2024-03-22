//
//  frist_app.cpp
//  VulkanProject
//
//  Created by zhang on 2023/12/20.
//

#include "keyboard_movement_controller.hpp"
#include "frist_app.hpp"
#include "test_camera.hpp"
#include "simple_render_system.hpp"
#include "test_buffer.hpp"
#include "point_light_system.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>
#include <chrono>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_SERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace test {


struct GlobalUbo {
    glm::mat4 projection{1.f};
    glm::mat4 view{1.f};
    glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .02f};  // w is intensity
    glm::vec3 lightPosition{-1.f};
    alignas(16) glm::vec4 lightColor{1.f};  // w is light intensity
};

FirstApp::FirstApp() {
    globalPool = TestDescriptorPool::Builder(testDevice)
            .setMaxSets(TestSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, TestSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
    loadGameObjects();
}

// temporary helper function, creates a 1x1x1 cube centered at offset
std::unique_ptr<TestModel> createCubeModel(TestDevice& device, glm::vec3 offset) {
          
    TestModel::Builder modelBuilder{};
    modelBuilder.vertices = {
        // left face (white)
          {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
          {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
          {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
          //{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
          {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
          //{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

          // right face (yellow)
          {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
          {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
          {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
          //{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
          {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
          //{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

          // top face (orange, remember y axis points down)
          {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
          {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
          {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
          //{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
          {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
          //{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

          // bottom face (red)
          {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
          {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
          {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
          //{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
          {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
          //{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

          // nose face (blue)
          {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
          {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
          {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
          //{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
          {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
          //{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

          // tail face (green)
          {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
          {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
          {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
          //{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
          {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
          //{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

    };

    for (auto& v : modelBuilder.vertices) {
      v.position += offset;
    }
    
    modelBuilder.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                              12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
    
    return std::make_unique<TestModel>(device, modelBuilder);
    
}

FirstApp::~FirstApp() {
}

void FirstApp::run(){
    
    std::vector<std::unique_ptr<TestBuffer>> uboBuffers(TestSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++) {
      uboBuffers[i] = std::make_unique<TestBuffer>(
          testDevice,
          sizeof(GlobalUbo),
          1,
          VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
      uboBuffers[i]->map();
    }
    
    auto globalSetLayout =
        TestDescriptorSetLayout::Builder(testDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

    std::vector<VkDescriptorSet> globalDescriptorSets(TestSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); i++) {
      auto bufferInfo = uboBuffers[i]->descriptorInfo();
      TestDescriptorWriter(*globalSetLayout, *globalPool)
          .writeBuffer(0, &bufferInfo)
          .build(globalDescriptorSets[i]);
    }

    SimpleRenderSystem simpleRenderSystem{
        testDevice,
        testRenderer.getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()};
    PointLightSystem pointLightSystem{
        testDevice,
        testRenderer.getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()
    };
    
    TestCamera camera{};
    
    auto viewerObject = TestGameObject::createGameObject();
    viewerObject.transform.translation.z = -2.5f;
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();
    while (!testWindow.shouldClose()) {
        glfwPollEvents();
        
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        cameraController.moveInPlaneXZ(testWindow.getGLFWwindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
        
        float aspect = testRenderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
        
        if (auto commandBuffer = testRenderer.beginFrame()) {
            int frameIndex = testRenderer.getFrameIndex();
            FrameInfo frameInfo{
                frameIndex,
                frameTime,
                commandBuffer,
                camera,
                globalDescriptorSets[frameIndex],
                gameObjects
            };
            
            //simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects,camera);
            // update
            GlobalUbo ubo{};
            ubo.projection = camera.getProjection();
            ubo.view = camera.getView();
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            // render
            testRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(frameInfo);
            pointLightSystem.render(frameInfo);
            testRenderer.endSwapChainRenderPass(commandBuffer);
            testRenderer.endFrame();
        }
        
    }
    
    vkDeviceWaitIdle(testDevice.device());
}

void FirstApp::loadGameObjects() {
    
    
    std::shared_ptr<TestModel> testModel = TestModel::createModelFromFile(testDevice, "models/flat_vase.obj");
    /*
    auto flatVase = TestGameObject::createGameObject();
    flatVase.model = testModel;
    flatVase.transform.translation = {-.5f, .5f, 0.f};
    flatVase.transform.scale = {3.f, 1.5f, 3.f};
    gameObjects.emplace(flatVase.getId(), std::move(flatVase));

    
    testModel = TestModel::createModelFromFile(testDevice, "models/smooth_vase.obj");
    auto smoothVase = TestGameObject::createGameObject();
    smoothVase.model = testModel;
    smoothVase.transform.translation = {.5f, .5f, 0.f};
    smoothVase.transform.scale = glm::vec3(3.f, 1.5f, 3.f);
    gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));*/
    
    testModel = TestModel::createModelFromFile(testDevice, "models/tank.obj");
    auto floor = TestGameObject::createGameObject();
    floor.model = testModel;
    floor.transform.translation = {-.5f, .5f, 0.f};
    floor.transform.scale = {0.1f, 0.1f, 0.1f};
    floor.transform.rotation = {0.f, -1.57f, 3.14f};
    gameObjects.emplace(floor.getId(), std::move(floor));
    
}

}
