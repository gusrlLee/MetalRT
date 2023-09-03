#include <iostream>
#include "Application.hpp"

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

int main(int args, char* argc[])
{
    int width = 720;
    int height = 480;
    
    Application app;
    app.Init(width, height);
    app.start();

    return 0;
}
