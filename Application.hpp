//
//  Application.hpp
//  MetalRT
//
//  Created by 이현기 on 9/1/23.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include <iostream>
#include <simd/simd.h>
// for Metal API
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <Metal/shared_ptr.hpp>
#include <QuartzCore/QuartzCore.hpp>


// for window 
#include "SDL2/SDL.h"
#include "Config.hpp"
#include "BVH.hpp"

class Application
{
public:
    Application() { }
    ~Application();
    void Init(int width, int height);
    void start();
    void eventHandle();
    void buildBVH();
    void updateBVH();
    
private:
    // Main widnow of Application
    SDL_Window *m_window;
    // Renderer using Metal API in Main Window.
    // Renderer *m_renderer;
    // for get keyboard input data
    SDL_Event m_keyboard_input_data;

    
    // system flag
    bool m_system_status;
};

#endif /* APPLICATION_H */
