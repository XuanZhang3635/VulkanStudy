//
//  test_window.cpp
//  VulkanProject
//
//  Created by zhang on 2023/12/20.
//

#include <stdio.h>
#include "test_window.h"
#include <stdexcept>
namespace test {
TestWindow::TestWindow(int w, int h, std::string name): width{w}, height{h}, windowName{name}{
    initWindow();
}

TestWindow::~TestWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void TestWindow::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    
    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void TestWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }
}

void TestWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto testWindow = reinterpret_cast<TestWindow *>(glfwGetWindowUserPointer(window));
    testWindow->framebufferResized = true;
    testWindow->width = width;
    testWindow->height = height;
}

}
