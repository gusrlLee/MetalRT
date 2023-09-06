//
//  TLAS.cpp
//  MetalCPP
//
//  Created by 이현기 on 9/6/23.
//

#include "TLAS.hpp"

TLAS::TLAS(BVH* bvh_list, int N)
{
    m_blas = bvh_list;
    m_blas_count = N;
    
    m_tlas_node = (TLAS_node*)malloc(sizeof(TLAS_node) * 2 * N);
    m_node_used = 2;
}

void TLAS::build()
{
    m_tlas_node[m_node_used].left_blas = 0;
    m_tlas_node[m_node_used].aabb.min = vector3(-100.f, -100.f, -100.f);
    m_tlas_node[m_node_used].aabb.max = vector3(100.f, 100.f, 100.f);
    m_tlas_node[m_node_used++].is_leaf = true;
    
    m_tlas_node[m_node_used].left_blas = 1;
    m_tlas_node[m_node_used].aabb.min = vector3(-100.f, -100.f, -100.f);
    m_tlas_node[m_node_used].aabb.max = vector3(100.f, 100.f, 100.f);
    m_tlas_node[m_node_used++].is_leaf = true;
    
    // create a root node over the two leaf nodes;
    m_tlas_node[0].left_blas = 2;
    m_tlas_node[0].aabb.min = vector3(-100.f, -100.f, -100.f);
    m_tlas_node[0].aabb.max = vector3(100.f, 100.f, 100.f);
    m_tlas_node[0].is_leaf = false;
}
