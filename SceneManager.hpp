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


#include "SceneData.hpp"
#include "Camera.hpp"

class SceneManager
{
public:
    SceneData* getCurrentScene() { return m_current_scene_data; }
    SceneData* getNextScene() { return m_next_scene_data; }
    
    void setCurrentScene(SceneData* scene) { m_current_scene_data = scene; }
    void setNextScene(SceneData* scene) { m_next_scene_data = scene; }
    
private:
    SceneData* m_current_scene_data;
    SceneData* m_next_scene_data;
};

#endif /* SCENE_MANAGER_H */
