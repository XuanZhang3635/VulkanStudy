#!/bin/sh

#  compile.sh
#  VulkanProject
#
#  Created by zhang on 2023/12/20.
#  

/Users/zhang/VulkanSDK/1.3.239.0/macOS/bin/glslc shaders/simple_shader.vert -o shaders/simple_shader.vert.spv
/Users/zhang/VulkanSDK/1.3.239.0/macOS/bin/glslc shaders/simple_shader.frag -o shaders/simple_shader.frag.spv
