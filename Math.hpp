//
//  Math.h
//  MetalRT
//
//  Created by 이현기 on 9/2/23.
//

#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <simd/simd.h>

inline vector_float3 fminf(vector_float3 &vector1, vector_float3 &vector2)
{
    return vector3(std::min(vector1.x, vector2.x), std::min(vector1.y, vector2.y), std::min(vector1.z, vector2.z));
}

inline vector_float3 fmaxf(vector_float3 &vector1, vector_float3 &vector2)
{
    return vector3(std::max(vector1.x, vector2.x), std::max(vector1.y, vector2.y), std::max(vector1.z, vector2.z));
}

#endif /* MATH_H */
