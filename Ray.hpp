//
//  Ray.h
//  MetalRT
//
//  Created by 이현기 on 9/6/23.
//

#ifndef RAY_H
#define RAY_H

#include <iostream>
#include <simd/simd.h>

typedef struct Ray {
    vector_float3 o, d, rd;
    float t = 1e30f;
} Ray;

#endif /* RAY_H */
