//
//  Timer.cpp
//  MetalRT
//
//  Created by 이현기 on 9/2/23.
//

#include <chrono>

#include "Timer.hpp"

float GetTime()
{
    return std::chrono::time_point_cast<std::chrono::microseconds>( std::chrono::high_resolution_clock::now() ).time_since_epoch().count();
}
