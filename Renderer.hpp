//
//  Tracer.hpp
//  MetalRT
//
//  Created by 이현기 on 9/3/23.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <Metal/shared_ptr.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <simd/simd.h>

#include "SDL2/SDL.h"

#include <simd/simd.h>

// Buffer index values shared between shader and C code to ensure Metal shader buffer inputs
// match Metal API buffer set calls.
typedef enum AAPLVertexInputIndex
{
    AAPLVertexInputIndexVertices     = 0,
    AAPLVertexInputIndexViewportSize = 1,
} AAPLVertexInputIndex;

//  This structure defines the layout of vertices sent to the vertex
//  shader. This header is shared between the .metal shader and C code, to guarantee that
//  the layout of the vertex array in the C code matches the layout that the .metal
//  vertex shader expects.
typedef struct
{
    vector_float2 position;
    vector_float4 color;
} AAPLVertex;

class Renderer
{
    
public:
    Renderer(SDL_Window* window);
    ~Renderer();
    void init(SDL_Window *window);
    void loadMetalShaderCodeFile(const char* metal_shader_code_file_name);
    void buildShader();
    void buildBuffer();
    void draw();
    
private:
    //
    SDL_Renderer *m_renderer;
    CA::MetalLayer* m_layer;
    
    // core device
    MTL::Device* m_device;
    MTL::Library* m_library;
    
    MTL::Function* m_vertex_function;
    MTL::Function* m_fragment_function;
    MTL::Function* m_kernel_function;
    MTL::RenderPipelineDescriptor* m_render_pipeline_descriptor;
    MTL::RenderPipelineState* m_render_pipeline_state;
    MTL::CommandQueue *m_command_queue;
    
    MTL::Buffer* m_vertex_poition_buffer;
    MTL::Buffer* m_vertex_color_buffer;
    
    // shader code name and info
    std::string m_metal_shader_code;
    
    const char* m_vertex_shader_code_name;
    const char* m_vertex_shader_code;
    
    const char* m_fragment_shader_code_name;
    const char* m_fragment_shader_code;
    
    const char* m_metal_kernel_trace_code_name;
    const char* m_metal_kernel_trace_code;
};

#endif /* RENDERER_H */
