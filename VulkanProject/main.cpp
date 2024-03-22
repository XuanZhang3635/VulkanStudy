/**
 -  GLFW: It provides a simple, platform-independent API for creating windows, contexts and surfaces, reading input, handling events, etc.
 - https://vulkan-tutorial.com/
 - GLM:  only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
 **/
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include "frist_app.hpp"

int main() {
    test::FirstApp app{};
    
    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
        
    }

    return EXIT_SUCCESS;
}
