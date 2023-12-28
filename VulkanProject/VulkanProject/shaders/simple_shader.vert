#version 450 // glsl 4.5
#extension GL_ARB_separate_shader_objects : enable

/**
 
 vec2 positions[3] = vec2[] (
     vec2(0.0,-0.5),
     vec2(0.5,0.5),
     vec2(-0.5,0.5)
 );
 
 */

//  run for every vertex
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

// 对应location=0的位置的颜色
layout(location = 0) out vec3 fragColor;

void main(){
    gl_Position = vec4(position, 0.0, 1.0);
    fragColor = color;
}
