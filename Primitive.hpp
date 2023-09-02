//
//  Primitive.h
//  MetalRT
//
//  Created by 이현기 on 9/2/23.
//

#ifndef Primitive_h
#define Primitive_h

#include <iostream>
#include <simd/simd.h>

typedef struct Sphere {
    float r;
    vector_float3 center;
} Sphere;

typedef struct Triangle {
    vector_float3 vertex[3];
    vector_float3 centeroid;
} Triangle;


#endif /* Primitive_h */
