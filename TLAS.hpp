//
//  TLAS.hpp
//  MetalCPP
//
//  Created by 이현기 on 9/6/23.
//

#ifndef TLAS_H
#define TLAS_H

#include <stdio.h>
#include <iostream>
#include <simd/simd.h>

#include "BVH.hpp"
#include "Primitive.hpp"

typedef struct TLAS_node {
    AABB3f aabb;
    uint left_blas;
    uint is_leaf;
} TLAS_node;

class TLAS
{
public:
    TLAS() = default;
    TLAS(BVH* bvh_list, int N);
    void build();
    
private:
    TLAS_node *m_tlas_node = 0;
    BVH* m_blas = 0;
    uint m_node_used, m_blas_count;
};

#endif /* TLAS_H */
