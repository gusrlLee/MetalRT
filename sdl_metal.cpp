    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("MetalRT", -1, -1, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    
    CA::MetalLayer* layer = (CA::MetalLayer*)SDL_RenderGetMetalLayer(renderer);
    
    auto device = layer->device();
    MTL::CommandQueue* queue = device->newCommandQueue();
    
    std::string metal_shader_code;
    std::ifstream metal_shader_file;
    metal_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        metal_shader_file.open("../Shader/shader.metal");
        std::stringstream shader_stream;
        shader_stream << metal_shader_file.rdbuf();
        metal_shader_file.close();
        
        // convert stream into string
        metal_shader_code = shader_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    
    NS::Error* p_error = nullptr;
    MTL::Library* library = device->newLibrary( NS::String::string(metal_shader_code.c_str(), NS::StringEncoding::UTF8StringEncoding), nullptr, &p_error );
    if ( !library )
    {
        __builtin_printf( "%s", p_error->localizedDescription()->utf8String() );
        assert( false );
    }
    
    MTL::Function* vertex_function = library->newFunction(NS::String::string("vertexMain", NS::StringEncoding::UTF8StringEncoding));
    MTL::Function* fragment_function = library->newFunction(NS::String::string("fragmentMain", NS::StringEncoding::UTF8StringEncoding));
    
    MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init();
    pDesc->setVertexFunction( vertex_function );
    pDesc->setFragmentFunction( fragment_function );
    pDesc->colorAttachments()->object(0)->setPixelFormat( layer->pixelFormat() );
    
    MTL::RenderPipelineState* pPSO = device->newRenderPipelineState( pDesc, &p_error );
    if ( !pPSO )
    {
        __builtin_printf( "%s", p_error->localizedDescription()->utf8String() );
        assert( false );
    }
    
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

    const size_t positionsDataSize = NumVertices * sizeof( simd::float3 );
    const size_t colorDataSize = NumVertices * sizeof( simd::float3 );

    MTL::Buffer* pVertexPositionsBuffer = device->newBuffer( positionsDataSize, MTL::ResourceStorageModeManaged );
    MTL::Buffer* pVertexColorsBuffer = device->newBuffer( colorDataSize, MTL::ResourceStorageModeManaged );

    MTL::Buffer* _pVertexPositionsBuffer = pVertexPositionsBuffer;
    MTL::Buffer* _pVertexColorsBuffer = pVertexColorsBuffer;

    memcpy( _pVertexPositionsBuffer->contents(), positions, positionsDataSize );
    memcpy( _pVertexColorsBuffer->contents(), colors, colorDataSize );

    _pVertexPositionsBuffer->didModifyRange( NS::Range::Make( 0, _pVertexPositionsBuffer->length() ) );
    _pVertexColorsBuffer->didModifyRange( NS::Range::Make( 0, _pVertexColorsBuffer->length() ) );
    
    bool quit = false;
    SDL_Event e;

    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT: {
                    quit = true;
                } break;
            }
        }

        auto drawable = layer->nextDrawable();

        auto pass = MTL::make_owned(MTL::RenderPassDescriptor::renderPassDescriptor());

        auto color_attachment = pass->colorAttachments()->object(0);
        color_attachment->setLoadAction(MTL::LoadAction::LoadActionClear);
        color_attachment->setStoreAction(MTL::StoreAction::StoreActionStore);
        color_attachment->setTexture(drawable->texture());

        //
//        auto buffer = MTL::make_owned(queue->commandBuffer());
        MTL::CommandBuffer* buffer = queue->commandBuffer();
        //
//        auto encoder = MTL::make_owned(buffer->renderCommandEncoder(pass.get()));
        MTL::RenderCommandEncoder* encoder = buffer->renderCommandEncoder(pass.get());

        
        encoder->setRenderPipelineState(pPSO);
        
        encoder->setVertexBuffer( _pVertexPositionsBuffer, 0, 0 );
        encoder->setVertexBuffer( _pVertexColorsBuffer, 0, 1 );
        
        NS::UInteger vertex_start = 0, vertex_count = 3;
        encoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, vertex_start, vertex_count);

        encoder->endEncoding();

        buffer->presentDrawable(drawable);
        buffer->commit();

        drawable->release();
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
