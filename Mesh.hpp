//
//  Mesh.hpp
//  MetalRT
//
//  Created by 이현기 on 9/4/23.
//

#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <iostream>

#include "Primitive.hpp"
//#include "BVH.hpp"

class Mesh
{
public:
    Mesh() = default;
    Mesh( const char* obj_file_name, const char* texture_file_name );
    
    Triangle m_triangle_list[1024];
    TriangleEx m_triangleEx_list[1024];
    int m_tri_count = 0;
    //BVH *m_bvh;
};


#endif /* MESH_H */
