//
//  BVH.hpp
//  MetalRT
//
//  Created by 이현기 on 9/4/23.
//

#ifndef BVH_H
#define BVH_H

#include <stdio.h>
#include <iostream>
#include <algorithm>

#include "Primitive.hpp"
#include "Math.hpp"
#include "Timer.hpp"

#define BINS 8

typedef struct Bin
{
    AABB3f bounds;
    int tri_count = 0;
} Bin;

typedef struct BVH_node
{
    AABB3f aabb;
    int tri_count;
    int left_first;
    bool isLeaf() { return tri_count > 0; }
    float calculateNodeCost()
    {
        vector_float3 e = aabb.max - aabb.min;
        return (e.x * e.y + e.y * e.z + e.z * e.x) * tri_count;
    }
    
} BVH_node;

class BVH
{
public:
    BVH() = default;
    BVH(const char* tri_file, int N);
    void build();
    void refit();
    void setTransform( matrix_float4x4 transform );
    void del();
    // void intersect( Ray &ray ) ;
    
private:
    void swap( const uint a, const uint b )
    {
        uint t = m_tri_index_list[a];
        m_tri_index_list[a] = m_tri_index_list[b];
        m_tri_index_list[b] = t;        
    }
    void subdivide( uint node_index );
    void updateNodeBounds( uint node_index );
    float findBestSplitPlane(BVH_node &node, int &axis, float &split_position);
    
    BVH_node* m_nodes = 0;
    Triangle* m_tri = 0;
    uint* m_tri_index_list = 0;
    uint m_nodes_used, m_tri_count;
    
    matrix_float4x4 m_inv_transform_mat;
    AABB3f m_bounds;
};

#endif /* BVH_H */
