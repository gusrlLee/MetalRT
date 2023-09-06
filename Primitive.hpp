//
//  Primitive.h
//  MetalRT
//
//  Created by 이현기 on 9/2/23.
//

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <iostream>
#include <simd/simd.h>

#include "Math.hpp"

typedef struct Sphere {
    float r;
    vector_float3 center;
} Sphere;

typedef struct Triangle {
    vector_float3 vertex[3];
    vector_float3 centroid;
    vector_float2 uv0, uv1, uv2; // texture coordinate
    vector_float3 N0, N1, N2; // normal coordinate
} Triangle;

// additional triangle data, for texturing and shading
typedef struct TriangleEx
{
    vector_float2 uv0, uv1, uv2;
    vector_float3 N0, N1, N2;
    float dummy;
} TriangleEx;

typedef struct AABB3f {
    vector_float3 min = 1e30f;
    vector_float3 max = -1e30f;
    void grow(vector_float3 p) { min = fminf(min, p), max = fmaxf(max, p); }
    void grow( AABB3f& b ) { if (b.min.x != 1e30f) { grow( b.min ); grow( b.max ); } }
    float Area()
    {
        float e[3];
        e[0] = max.x - min.x;
        e[1] = max.y - min.y;
        e[2] = max.z - min.z;

        return e[0] * e[1] + e[1] * e[2] + e[2] * e[0]; // return e.x * e.y + e.y * e.z + e.z * e.x;
    }
} AABB3f;

#endif /* PRIMITIVE_H */
