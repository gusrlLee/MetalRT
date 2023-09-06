//
//  BVH.cpp
//  MetalRT
//
//  Created by 이현기 on 9/1/23.
//

#include "BVH_temp.hpp"

inline void SWAP(Triangle &Tri1, Triangle &Tri2)
{
    Triangle temp;
    temp = Tri1;
    Tri1 = Tri2;
    Tri2 = temp;
}

void BVH_build(BVH_data* bvh, int total_number_of_triagles)
{
    // root node & bvh info setting
    bvh->root_index = 0;
    bvh->total_number_of_triangles = total_number_of_triagles;
    bvh->nodes[bvh->root_index].left_first = 0;
    bvh->nodes[bvh->root_index].tri_count = total_number_of_triagles;
    bvh->used_nodes = 1;
    
    // construction start
    BVH_updateBoundBox(bvh, bvh->root_index);
    BVH_subdivide(bvh, bvh->root_index);
}

void BVH_del(BVH_data* bvh)
{
    if (bvh->triangle_list != NULL)
        free(bvh->triangle_list);
    if (bvh->nodes != NULL)
        free(bvh->nodes);
    if (bvh != NULL)
        free(bvh);
}

void BVH_updateBoundBox(BVH_data* bvh, int node_index)
{
    vector_float3 min = vector3(1e30f, 1e30f, 1e30f);
    vector_float3 max = vector3(-1e30f, -1e30f, -1e30f);
    BVH_node& node = bvh->nodes[node_index];
    
    for (int first = node.left_first, i=0; i<node.tri_count; i++)
    {
        Triangle leaf_triangle = bvh->triangle_list[first + i];
        vector_float3 leaf_triangle_vertex_0 = vector3(leaf_triangle.vertex[0].x, leaf_triangle.vertex[0].y, leaf_triangle.vertex[0].z);
        vector_float3 leaf_triangle_vertex_1 = vector3(leaf_triangle.vertex[1].x, leaf_triangle.vertex[1].y, leaf_triangle.vertex[1].z);
        vector_float3 leaf_triangle_vertex_2 = vector3(leaf_triangle.vertex[2].x, leaf_triangle.vertex[2].y, leaf_triangle.vertex[2].z);
        
        min = fminf(min, leaf_triangle_vertex_0);
        min = fminf(min, leaf_triangle_vertex_1);
        min = fminf(min, leaf_triangle_vertex_2);
        
        max = fmaxf(max, leaf_triangle_vertex_0);
        max = fmaxf(max, leaf_triangle_vertex_1);
        max = fmaxf(max, leaf_triangle_vertex_2);
    }
    node.aabb.max = max;
    node.aabb.min = min;
}

void BVH_subdivide(BVH_data* bvh, int node_index)
{
    // get current node
    BVH_node& current_bvh_node = bvh->nodes[node_index];

    int axis;
    float split_position;
    float split_cost = BVH_findBestSplitPlane(current_bvh_node, bvh->triangle_list, axis, split_position);
    float no_split_cost = BVH_calculatedNodeCost(current_bvh_node);
    
    if (split_cost >= no_split_cost) return;

    // in-place partition
    int i = current_bvh_node.left_first;
    int j = i + current_bvh_node.tri_count - 1;
    while( i <= j )
    {
        if (bvh->triangle_list[i].centeroid[axis] < split_position )
            i++;
        else
            SWAP(bvh->triangle_list[i], bvh->triangle_list[j--]);
    }

    // abort split if one of the sides is empty
    int left_count = i - current_bvh_node.left_first;
    if (left_count == 0 || left_count == current_bvh_node.tri_count) return;

    // make child node index
    int left_child_index = bvh->used_nodes++;
    int right_child_index = bvh->used_nodes++;

    bvh->nodes[left_child_index].left_first= current_bvh_node.left_first;
    bvh->nodes[left_child_index].tri_count = left_count;

    bvh->nodes[right_child_index].left_first = i;
    bvh->nodes[right_child_index].tri_count = current_bvh_node.tri_count - left_count;

    current_bvh_node.left_first = left_child_index;
    current_bvh_node.tri_count = 0; // mark inner node

    // call again
    BVH_updateBoundBox(bvh, left_child_index);
    BVH_updateBoundBox(bvh, right_child_index);

    BVH_subdivide(bvh, left_child_index);
    BVH_subdivide(bvh, right_child_index);
}

float BVH_evaluationSAH(BVH_node &node, Triangle *triangle_vertex_list, int axis, float pos)
{
    AABB3f left_bounding_box, right_bounding_box;

    left_bounding_box.min = vector3(1e30f, 1e30f, 1e30f);
    left_bounding_box.max = vector3(-1e30f, -1e30f, -1e30f);
    
    right_bounding_box.min = vector3(1e30f, 1e30f, 1e30f);
    right_bounding_box.max = vector3(-1e30f, -1e30f, -1e30f);

    int leftCount = 0, rightCount = 0;

    for (int i=0; i<node.tri_count; i++)
    {
        
        Triangle &tri = triangle_vertex_list[node.left_first + i];
        vector_float3 triangle_vertex_0 = vector3(tri.vertex[0].x, tri.vertex[0].y, tri.vertex[0].z);
        vector_float3 triangle_vertex_1 = vector3(tri.vertex[1].x, tri.vertex[1].y, tri.vertex[1].z);
        vector_float3 triangle_vertex_2 = vector3(tri.vertex[2].x, tri.vertex[2].y, tri.vertex[2].z);

        if (tri.centeroid[axis] < pos)
        {
            leftCount++;
            left_bounding_box.grow(triangle_vertex_0);
            left_bounding_box.grow(triangle_vertex_1);
            left_bounding_box.grow(triangle_vertex_2);
        }
        else
        {
            rightCount++;
            right_bounding_box.grow(triangle_vertex_0);
            right_bounding_box.grow(triangle_vertex_1);
            right_bounding_box.grow(triangle_vertex_2);
        }
    }
    float cost = (float)leftCount * left_bounding_box.Area() + (float)rightCount * right_bounding_box.Area();
    float temp = cost > 0 ? cost : 1e30f;
    return cost > 0 ? cost : 1e30f;
}

float BVH_findBestSplitPlane(BVH_node &node, Triangle *triangle_vertex_list, int& axis, float &splitPos)
{
    float best_cost = 1e30f;
    for (int a=0; a<3; a++)
    {
        float bounding_box_min = 1e30f;
        float bounding_box_max = -1e30f;
        
        for (int i=0; i<node.tri_count; i++)
        {
            Triangle& triangle = triangle_vertex_list[node.left_first + i];
            bounding_box_min = std::min(bounding_box_min, triangle.centeroid[a]);
            bounding_box_max = std::max(bounding_box_max, triangle.centeroid[a]);
        }
        if (bounding_box_min == bounding_box_max) continue;
        
        float scale = (bounding_box_max - bounding_box_min) / 100;
        for (int i=1; i<100; i++)
        {
            float candidate_position = bounding_box_min + i * scale;
            float cost = BVH_evaluationSAH(node, triangle_vertex_list, a, candidate_position);
            if (cost < best_cost)
            {
                splitPos = candidate_position;
                axis = a;
                best_cost = cost;
            }
        }
    }
    return best_cost;
}

float BVH_calculatedNodeCost(BVH_node &node)
{
    vector_float3 e = node.aabb.max - node.aabb.min;
    float surface_area = e.x * e.y + e.y * e.z + e.z * e.x;
    return node.tri_count * surface_area;
}
