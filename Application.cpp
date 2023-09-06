//
//  Application.cpp
//  MetalRT
//
//  Created by 이현기 on 9/1/23.
//

#include "Application.hpp"
#include "Timer.hpp"

Application::~Application()
{
//    delete m_player;
//    delete m_renderer;
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Application::Init(int width, int height)
{
    printf("MetalRT Init.... \n ");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    m_window = SDL_CreateWindow("MetalRT", -1, -1, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
//    m_renderer = new Renderer(m_window);
    
    vector_float3 camera_position;
    camera_position.x = -1.5f;
    camera_position.y = -0.2f;
    camera_position.z = -2.5f;
    
    vector_float3 p[3];
    p[0].x = -1; p[0].y =  1; p[0].z = 2;
    p[1].x =  1; p[1].y =  1; p[1].z = 2;
    p[2].x = -1; p[2].y = -1; p[2].z = 2;
    
    // setting define
//    m_player = new Camera(p, camera_position);
    
    m_system_status = ON; // status on'
    printf("Done.\n");
}

// rendering source
void Application::start()
{
    // before prcessing of rendering, we pre-construction BVH data.
    buildBVH();
    
    while (m_system_status) {
        // input data operation
        eventHandle();
        
        // if location of mesh is transformed, we have to update bvh info
        updateBVH();
        
        // Trace ray.
//        m_renderer->draw();
    }
}

void Application::eventHandle()
{
    while (SDL_PollEvent(&m_keyboard_input_data) != 0) {
        switch (m_keyboard_input_data.type) {
            case SDL_QUIT:
            {
                m_system_status = OFF;
            } break;
        }
    }
}

void Application::buildBVH()
{
    std::string file_name = "../Assets/armadillo.tri";
    BVH bvh = BVH( file_name.c_str(), 30000 );
    std::cout << "BVH_construction Done .. " << std::endl;
}

void Application::updateBVH()
{
    
}
