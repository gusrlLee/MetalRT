//
//  Camera.hpp
//  MetalRT
//
//  Created by 이현기 on 9/1/23.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <iostream>
#include <simd/simd.h>

class Camera
{
public:
    Camera(vector_float3 *screen_points, vector_float3 camera_location);
private:
    vector_float3 m_screen_points[3];
    vector_float3 m_lower_left_corner;
    vector_float3 m_camera_location;
};
#endif /* CAMERA_H */
