//
//  Scene.hpp
//  MetalRT
//
//  Created by 이현기 on 9/1/23.
//

#ifndef SCENE_DATA_H
#define SCENE_DATA_H

#include <stdio.h>
#include <iostream>

#include "BVH.hpp"
#include "Primitive.hpp"

typedef BVH BVH_Data;

class SceneData
{
public:
    

private:
    int m_scene_index;
    Triangle *m_triangle_list;
    BVH_node *m_bvh_nodes;
    BVH* m_bvh;
};

#endif /* SCENE_DATA_H */
