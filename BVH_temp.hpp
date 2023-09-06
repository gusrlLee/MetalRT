//
//  BVH.hpp
//  MetalRT
//
//  Created by 이현기 on 9/1/23.
//

#ifndef BVH_H
#define BVH_H

#include <stdio.h>
#include <iostream>
#include <simd/simd.h>

#include "Primitive.hpp"
#include "Math.hpp"

typedef struct AABB3f {
    vector_float3 min;
    vector_float3 max;
    void grow(vector_float3 p) { min = fminf(min, p), max = fmaxf(max, p); }
    float Area()
    {
        float e[3];
        e[0] = max.x - min.x;
        e[1] = max.y - min.y;
        e[2] = max.z - min.z;

        return e[0] * e[1] + e[1] * e[2] + e[2] * e[0]; // return e.x * e.y + e.y * e.z + e.z * e.x;
    }
} AABB3f;

typedef struct BVH_node {
    AABB3f aabb;
    int left_first, tri_count;
    bool isLeaf() { return tri_count > 0; }
} BVH_node;

typedef struct BVH {
    int root_index;
    int total_number_of_triangles;
    int used_nodes;
    BVH_node *nodes;
    Triangle *triangle_list;
} BVH_data;

void BVH_build(BVH_data* bvh, int total_number_of_triagles);
void BVH_del(BVH_data* bvh);
void BVH_updateBoundBox(BVH_data* bvh, int node_index);
void BVH_subdivide(BVH_data* bvh, int node_index);
float BVH_evaluationSAH(BVH_node &node, Triangle *triangle_vertex_list, int axis, float pos);

float BVH_findBestSplitPlane(BVH_node &node, Triangle *triangle_vertex_list, int& axis, float &splitPos);
float BVH_calculatedNodeCost(BVH_node &node);
#endif /* BVH_H */
