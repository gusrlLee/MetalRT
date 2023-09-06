//
//  Mesh.cpp
//  MetalRT
//
//  Created by 이현기 on 9/4/23.
//

#include "Mesh.hpp"

Mesh::Mesh( const char* obj_file_name, const char* texture_file_name )
{
    vector_float2 UV[1024];
    vector_float3 N[1024], P[1024];
    int UVs = 0, Ns = 0, Ps = 0, a, b, c, d, e, f, g, h, i;
    float aa, bb, cc, dd, ee, ff, gg, hh;
    
    FILE* file = fopen( obj_file_name, "r" );
    
    while (!feof( file ))
    {
        char line[256] = { 0 };
        fgets( line, 1023, file );
        if (line == strstr( line, "vt " ))
        {
            UVs++;
            sscanf(line + 3, "%f %f", &aa, &bb);
            UV[UVs].x = aa;
            UV[UVs].y = bb;
        }
        else if (line == strstr( line, "vn " ))
        {
            Ns++;
            sscanf( line + 3, "%f %f %f", &cc, &dd, &ee );
            N[Ns].x = cc;
            N[Ns].y = dd;
            N[Ns].z = ee;
        }
        else if (line[0] == 'v')
        {
            Ps++,
            sscanf( line + 2, "%f %f %f", &ff, &gg, &hh );
            P[Ps].x = ff;
            P[Ps].y = gg;
            P[Ps].z = hh;
            
        }
        if (line[0] != 'f') continue; else
        sscanf( line + 2, "%i/%i/%i %i/%i/%i %i/%i/%i",
            &a, &b, &c, &d, &e, &f, &g, &h, &i );
        m_triangle_list[m_tri_count].vertex[0] = P[a], m_triangleEx_list[m_tri_count].N0 = N[b];
        m_triangle_list[m_tri_count].vertex[1] = P[d], m_triangleEx_list[m_tri_count].N1 = N[e];
        m_triangle_list[m_tri_count].vertex[2] = P[g], m_triangleEx_list[m_tri_count].N2 = N[h];
        m_triangleEx_list[m_tri_count].uv0 = UV[c], m_triangleEx_list[m_tri_count].uv1 = UV[f];
        m_triangleEx_list[m_tri_count++].uv2 = UV[i];
    }
    
    fclose(file);
    
    m_bvh = new BVH(this);
    
}
