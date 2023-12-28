//
//  test_window.h
//  VulkanProject
//
//  Created by zhang on 2023/12/20.
//

#ifndef test_window_h
#define test_window_h

# pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
namespace test {

class TestWindow{
    
public:
    TestWindow(int w, int h, std::string name);
    ~TestWindow();
    
    // 禁用了通过拷贝构造函数创建对象的操作，保证只有一个window
    TestWindow(const TestWindow &) = delete;
    TestWindow &operator=(const TestWindow &) = delete;
    
    bool shouldClose() {return glfwWindowShouldClose(window);}
    VkExtent2D getExtent() { return {static_cast<uint32_t>(width),static_cast<uint32_t>(height)}; }
    bool wasWindowResized() {return framebufferResized;}
    void resetWindowResizedFlag() {framebufferResized = false;}
    
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    
private:
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
    void initWindow();
    int width;
    int height;
    bool framebufferResized = false;
    
    std::string windowName;
    GLFWwindow *window;
    
};

}
#endif /* test_window_h */
