#include <iostream>
#include "SDL2/SDL.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <Metal/shared_ptr.hpp>
#include <QuartzCore/QuartzCore.hpp>

int main()
{
    std::cout << "Hello world" << std::endl;
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDL Metal", -1, -1, 720, 480, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    NS::Error *err;

    auto swapchain = (CA::MetalLayer*)SDL_RenderGetMetalLayer(renderer);
    auto device = swapchain->device();

    auto name = device->name();
    std::cerr << "device name: " << name->utf8String() << std::endl;


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
