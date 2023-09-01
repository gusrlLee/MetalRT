//
//  Application.hpp
//  MetalRT
//
//  Created by 이현기 on 9/1/23.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <iostream>
// for Metal API
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <Metal/shared_ptr.hpp>
#include <QuartzCore/QuartzCore.hpp>

// for window 
#include "SDL2/SDL.h"

class Application
{
public:
    Application() { }
    ~Application();
    void Init(int width, int height);
    void start();
    
private:
    // Main widnow of Application
    SDL_Window *mWindow;
    // Renderer using Metal API in Main Window.
    SDL_Renderer *mRenderer;
};

#endif /* Application_hpp */
