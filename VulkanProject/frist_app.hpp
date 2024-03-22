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
#include "test_renderer.hpp"
#include "test_swap_chain.hpp"
#include "test_game_object.hpp"
#include "test_descriptor.hpp"

// std
#include <memory>
#include <vector>

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
    void loadGameObjects();
    
    TestWindow testWindow{WIDTH, HIEGHT, "Hello Vulkan!"};
    TestDevice testDevice{testWindow};
    TestRenderer testRenderer{testWindow, testDevice};
    
    std::unique_ptr<TestDescriptorPool> globalPool{};
    TestGameObject::Map gameObjects;
};

}

#endif /* frist_app_hpp */
