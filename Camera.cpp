//
//  Camera.cpp
//  MetalRT
//
//  Created by 이현기 on 9/1/23.
//

#include "Camera.hpp"

Camera::Camera(vector_float3 *screen_points, vector_float3 camera_location)
{
    for(int i=0; i<3; i++)
    {
        m_screen_points[i] = screen_points[i];
    }
    m_camera_location = camera_location;
    
    m_lower_left_corner.x = m_screen_points[0].x - m_camera_location.x;
    m_lower_left_corner.y = m_screen_points[0].y - m_camera_location.y;
    m_lower_left_corner.z = m_screen_points[0].z - m_camera_location.z;
}
