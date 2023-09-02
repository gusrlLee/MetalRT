//
//  SceneManager.hpp
//  MetalRT
//
//  Created by 이현기 on 9/2/23.
//

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <stdio.h>
#include <iostream>
#include <unordered_map>


#include "BVH.hpp"
#include "SceneData.hpp"
#include "Camera.hpp"

class SceneManager
{
public:
    SceneData* getCurrentScene() { return m_scene_list[m_current_scene_index]; }
    
private:
    int m_current_scene_index;
    std::unordered_map<int, SceneData*> m_scene_list;
    
};

#endif /* SCENE_MANAGER_H */
