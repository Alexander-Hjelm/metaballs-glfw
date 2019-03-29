#version 400

layout(points) in;
layout(triangle_strip, max_vertices = 14) out;

in vData
{
    vec3 position;
} vertices[];

out fData
{
    vec3 position;
} frag;

uniform float voxelHalfLength;
uniform int metaballCount;
uniform mat4 MVP;
uniform sampler2D metaballPosTexture; 

const int INDICES[14] = int[14]( 4, 3, 7, 8, 5, 3, 1, 4, 2, 7, 6, 5, 2, 1 );    //  minimalistic way to define a unit cube

const float ISOSURFACE_INDEX = 2.5;

float potentialFieldModel(vec3 srcPos, vec3 measurePos, float ballRadius) {
    vec3 r = measurePos - srcPos;
    
    return ballRadius/(r.x*r.x + r.y*r.y + r.z*r.z);
}

void main() {

    float influence = 0.0f;
    bool outside = true;

    for(int i = 0; i < metaballCount; ++i)
    {
        vec4 rgba = texelFetch(metaballPosTexture, ivec2(0, i), 0).rgba;
        influence += potentialFieldModel(rgba.rgb, vertices[0].position, rgba.a);
        if(influence >= ISOSURFACE_INDEX)
        {
            outside = false;
            break;
        }
    }

    if(outside)
    {
        EndPrimitive();
        return;
    }

    // define eight corner vertices for the cube (voxel)
    vec3 pos[8];
    pos[0] = vertices[0].position + vec3(-voxelHalfLength, +voxelHalfLength, -voxelHalfLength);
    pos[1] = vertices[0].position + vec3(+voxelHalfLength, +voxelHalfLength, -voxelHalfLength);
    pos[2] = vertices[0].position + vec3(-voxelHalfLength, +voxelHalfLength, +voxelHalfLength);
    pos[3] = vertices[0].position + vec3(+voxelHalfLength, +voxelHalfLength, +voxelHalfLength);
    pos[4] = vertices[0].position + vec3(-voxelHalfLength, -voxelHalfLength, -voxelHalfLength);
    pos[5] = vertices[0].position + vec3(+voxelHalfLength, -voxelHalfLength, -voxelHalfLength);
    pos[6] = vertices[0].position + vec3(+voxelHalfLength, -voxelHalfLength, +voxelHalfLength);
    pos[7] = vertices[0].position + vec3(-voxelHalfLength, -voxelHalfLength, +voxelHalfLength);
    
    // push vertex to primitive
    for(int i = 0; i < 14; ++i)
    {
        frag.position = vertices[0].position;
        gl_Position = MVP * vec4(pos[INDICES[i]-1], 1.0);
        EmitVertex();
    }

    EndPrimitive();

}
