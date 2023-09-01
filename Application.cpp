//
//  Application.cpp
//  MetalRT
//
//  Created by 이현기 on 9/1/23.
//

#include "Application.hpp"

Application::~Application()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Application::Init(int width, int height)
{
    printf("MetalRT Init.... \n ");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    mWindow = SDL_CreateWindow("MetalRT", -1, -1, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    printf("Done.\n");
    
    auto swapchain = (CA::MetalLayer*)SDL_RenderGetMetalLayer(mRenderer);
    auto device = swapchain->device();

    auto name = device->name();
    std::cout << "device name: " << name->utf8String() << std::endl;
}

void Application::start()
{
    bool quit = false;
    SDL_Event e;
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                {
                    quit = true;
                } break;
            }
        }
    }
}

