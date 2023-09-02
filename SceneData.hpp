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
    Triangle* m_temp_data;
    BVH* m_bvh;
    float m_index;
    
};

#endif /* SCENE_DATA_H */
