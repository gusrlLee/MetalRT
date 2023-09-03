//
//  Tracer.cpp
//  MetalRT
//
//  Created by 이현기 on 9/3/23.
//

#include "Renderer.hpp"

Renderer::Renderer(SDL_Window* window)
{
    init(window);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(m_renderer);
}

void Renderer::init(SDL_Window* window)
{
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    m_layer = (CA::MetalLayer*)SDL_RenderGetMetalLayer(m_renderer);
    m_device = m_layer->device();
    
    m_command_queue = m_device->newCommandQueue();
    loadMetalShaderCodeFile("../Shader/shader.metal");
    buildShader();
    buildBuffer();
}

void Renderer::loadMetalShaderCodeFile(const char *metal_shader_code_file_name)
{
    std::string metal_shader_code;
    std::ifstream metal_shader_file;
    metal_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        metal_shader_file.open(metal_shader_code_file_name);
        std::stringstream shader_stream;
        shader_stream << metal_shader_file.rdbuf();
        metal_shader_file.close();
        
        // convert stream into string
        m_metal_shader_code = shader_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    
    NS::Error* p_error = nullptr;
    m_library = m_device->newLibrary( NS::String::string(m_metal_shader_code.c_str(), NS::StringEncoding::UTF8StringEncoding), nullptr, &p_error );
    if ( !m_library )
    {
        __builtin_printf( "%s", p_error->localizedDescription()->utf8String() );
        assert( false );
    }
    
    m_vertex_function = m_library->newFunction(NS::String::string("vertexMain", NS::StringEncoding::UTF8StringEncoding));
    m_fragment_function = m_library->newFunction(NS::String::string("fragmentMain", NS::StringEncoding::UTF8StringEncoding));
}

void Renderer::buildShader()
{
    NS::Error* pError = nullptr;
    
    m_render_pipeline_descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    m_render_pipeline_descriptor->setVertexFunction(m_vertex_function);
    m_render_pipeline_descriptor->setFragmentFunction(m_fragment_function);
    m_render_pipeline_descriptor->colorAttachments()->object(0)->setPixelFormat(m_layer->pixelFormat());
    
    m_render_pipeline_state = m_device->newRenderPipelineState(m_render_pipeline_descriptor, &pError);
    if ( !m_render_pipeline_state )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }
    
    m_vertex_function->release();
    m_fragment_function->release();
    m_render_pipeline_descriptor->release();
    m_library->release();
}

void Renderer::buildBuffer()
{
    const size_t NumVertices = 3;

    simd::float3 positions[NumVertices] =
    {
        { -0.8f,  0.8f, 0.0f },
        {  0.0f, -0.8f, 0.0f },
        { +0.8f,  0.8f, 0.0f }
    };

    simd::float3 colors[NumVertices] =
    {
        {  1.0, 0.3f, 0.2f },
        {  0.8f, 1.0, 0.0f },
        {  0.8f, 0.0f, 1.0 }
    };

    const size_t positions_data_size = NumVertices * sizeof( simd::float3 );
    const size_t color_data_size = NumVertices * sizeof( simd::float3 );

    MTL::Buffer* pVertexPositionsBuffer = m_device->newBuffer( positions_data_size, MTL::ResourceStorageModeManaged );
    MTL::Buffer* pVertexColorsBuffer = m_device->newBuffer( color_data_size, MTL::ResourceStorageModeManaged );

    m_vertex_poition_buffer = pVertexPositionsBuffer;
    m_vertex_color_buffer = pVertexColorsBuffer;

    memcpy( m_vertex_poition_buffer->contents(), positions, positions_data_size );
    memcpy( m_vertex_color_buffer->contents(), colors, color_data_size );

    m_vertex_poition_buffer->didModifyRange( NS::Range::Make( 0, m_vertex_poition_buffer->length() ) );
    m_vertex_color_buffer->didModifyRange( NS::Range::Make( 0, m_vertex_color_buffer->length() ) );
}

void Renderer::draw()
{
    auto drawable = m_layer->nextDrawable();
    MTL::CommandBuffer* pCmd = m_command_queue->commandBuffer();
    MTL::RenderPassDescriptor* pass2 = MTL::RenderPassDescriptor::renderPassDescriptor()->alloc()->init();

    auto color_attachment = pass2->colorAttachments()->object(0);
    color_attachment->setLoadAction(MTL::LoadAction::LoadActionClear);
    color_attachment->setStoreAction(MTL::StoreAction::StoreActionStore);
    color_attachment->setTexture(drawable->texture());
    
    MTL::RenderCommandEncoder* pEnc = pCmd->renderCommandEncoder( pass2 );

    pEnc->setRenderPipelineState( m_render_pipeline_state );
    pEnc->setVertexBuffer( m_vertex_poition_buffer, 0, 0 );
    pEnc->setVertexBuffer( m_vertex_color_buffer, 0, 1 );
    pEnc->drawPrimitives( MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3) );
    
    pEnc->endEncoding();
    pCmd->presentDrawable(drawable);
    pCmd->commit();

    drawable->release();
}
