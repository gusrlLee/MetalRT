//
//  BVH.cpp
//  MetalRT
//
//  Created by 이현기 on 9/4/23.
//

#include "BVH.hpp"

BVH::BVH(const char* tri_file, int N)
{
    FILE* file = fopen(tri_file, "r");
    m_tri_count = N;
    m_tri = new Triangle[N];
    float a, b, c, d, e, f, g, h, i, j;
    for (int t = 0; t<N; t++)
    {
        fscanf( file, "%f %f %f %f %f %f %f %f %f\n", &a, &b, &c, &d, &e, &f, &g, &h, &i);
        m_tri[t].vertex[0].x = a; m_tri[t].vertex[0].y = b; m_tri[t].vertex[0].z = c;
        m_tri[t].vertex[1].x = d; m_tri[t].vertex[1].y = e; m_tri[t].vertex[1].z = f;
        m_tri[t].vertex[2].x = g; m_tri[t].vertex[2].y = h; m_tri[t].vertex[2].z = i;
    }
    m_nodes = (BVH_node*)malloc(sizeof(BVH_node) * (2 * N));
    m_tri_index_list = new uint[N];
    build();
    fclose(file);
}

void BVH::del()
{
    if (m_nodes != NULL)
        free(m_nodes);
    if (m_tri_index_list != NULL)
        free(m_tri_index_list);
    if (m_tri != NULL)
        free(m_tri);
}

void BVH::build()
{
    // reset node pool
    m_nodes_used = 2;
    
    // triangle index array setting
    for (uint i=0; i<m_tri_count; i++)
        m_tri_index_list[i] = i;
    
    // calculate triangle centeroid for partitioning
    for (uint i=0; i<m_tri_count; i++)
    {
        m_tri[i].centroid.x = (m_tri[i].vertex[0].x + m_tri[i].vertex[1].x + m_tri[i].vertex[2].x) * 0.3333f;
        m_tri[i].centroid.y = (m_tri[i].vertex[0].y + m_tri[i].vertex[1].y + m_tri[i].vertex[2].y) * 0.3333f;
        m_tri[i].centroid.z = (m_tri[i].vertex[0].z + m_tri[i].vertex[1].z + m_tri[i].vertex[2].z) * 0.3333f;
    }
    
    // assign all triangles to root node
    BVH_node &root = m_nodes[0];
    root.left_first = 0;
    root.tri_count = m_tri_count;
    updateNodeBounds( 0 );
    auto start = GetTime();
    subdivide( 0 );
    auto end = GetTime();
    std::cout << "[LOG] BVH construction in " << (end - start) / 1000.0f << " ms" << std::endl;
}

void BVH::updateNodeBounds(uint node_index)
{
    BVH_node &node = m_nodes[node_index];
    node.aabb.min = vector3(1e30f, 1e30f, 1e30f);
    node.aabb.max = vector3(-1e30f, -1e30f, -1e30f);
    
    for (uint first=node.left_first, i=0; i<node.tri_count; i++)
    {
        uint leaf_tri_idx = m_tri_index_list[first + i];
        Triangle &leaf_tri = m_tri[leaf_tri_idx];
        node.aabb.min = fminf( node.aabb.min, leaf_tri.vertex[0] );
        node.aabb.min = fminf( node.aabb.min, leaf_tri.vertex[1] );
        node.aabb.min = fminf( node.aabb.min, leaf_tri.vertex[2] );
        
        node.aabb.max = fmaxf( node.aabb.max, leaf_tri.vertex[0] );
        node.aabb.max = fmaxf( node.aabb.max, leaf_tri.vertex[1] );
        node.aabb.max = fmaxf( node.aabb.max, leaf_tri.vertex[2] );
    }
}

float BVH::findBestSplitPlane(BVH_node &node, int &axis, float &split_position)
{
    float best_cost = 1e30f;
    for (int a=0; a<3; a++)
    {
        float bounds_min = 1e30f, bounds_max = -1e30f;
        for (uint i=0; i<node.tri_count; i++)
        {
            Triangle &triangle = m_tri[m_tri_index_list[node.left_first + i]];
            bounds_min = fmin(bounds_min, triangle.centroid[a]);
            bounds_max = fmax(bounds_max, triangle.centroid[a]);
        }
        
        if (bounds_min == bounds_max)
            continue;
        
        Bin bin[BINS];
        float scale = BINS / (bounds_max - bounds_min);
        for (uint i=0; i<node.tri_count; i++)
        {
            Triangle &triangle = m_tri[m_tri_index_list[node.left_first + i]];
            int bin_idx = std::min(BINS - 1, (int)((triangle.centroid[a] - bounds_min) * scale));
            bin[bin_idx].tri_count++;
            bin[bin_idx].bounds.grow( triangle.vertex[0] );
            bin[bin_idx].bounds.grow( triangle.vertex[1] );
            bin[bin_idx].bounds.grow( triangle.vertex[2] );
        }
        
        float left_area[BINS - 1], right_area[BINS - 1];
        int left_count[BINS - 1], right_count[BINS - 1];
        AABB3f left_box, right_box;
        int left_sum = 0, right_sum = 0;
        
        for(int i=0; i<BINS-1; i++)
        {
            left_sum += bin[i].tri_count;
            left_count[i] = left_sum;
            left_box.grow( bin[i].bounds );
            left_area[i] = left_box.Area();
            
            right_sum += bin[BINS - 1 -i].tri_count;
            right_count[BINS - 2 - i] = right_sum;
            right_box.grow(bin[BINS - 1 - i].bounds);
            right_area[BINS - 2 - i] = right_box.Area();
        }
        scale = (bounds_max - bounds_min) / BINS;
        
        for (int i=0; i<BINS-1; i++)
        {
            float plane_cost = left_count[i] * left_area[i] + right_count[i] * right_area[i];
            if (plane_cost < best_cost)
            {
                axis = a;
                split_position = bounds_min + scale * (i + 1);
                best_cost = plane_cost;
            }
        }
    }
    
    return best_cost;
}

void BVH::subdivide(uint node_index)
{
    BVH_node &node = m_nodes[node_index];
    
    int axis;
    float split_position;
    float split_cost = findBestSplitPlane(node, axis, split_position);
    float no_split_cost = node.calculateNodeCost();
    
    if (split_cost >= no_split_cost) return;
    
    int i = node.left_first;
    int j = i + node.tri_count - 1; // j 가 0으로 할당 되는 문제 발견.
    
    while (i <= j)
    {
        if (m_tri[m_tri_index_list[i]].centroid[axis] < split_position)
            i++;
        else
//            swap( m_tri_index_list[i], m_tri_index_list[j--]);
            swap(i, j--);
    }
    
    int left_count = i - node.left_first;
    if (left_count == 0 || left_count == node.tri_count) return;
    
    int left_child_idx = m_nodes_used++;
    int right_child_idx = m_nodes_used++;
    
    m_nodes[left_child_idx].left_first = node.left_first;
    m_nodes[left_child_idx].tri_count = left_count;
    
    m_nodes[right_child_idx].left_first = i;
    m_nodes[right_child_idx].tri_count = node.tri_count - left_count;
    
    node.left_first = left_child_idx;
    node.tri_count = 0;
    
    updateNodeBounds( left_child_idx );
    updateNodeBounds( right_child_idx );
    
    subdivide( left_child_idx );
    subdivide( right_child_idx );
}
