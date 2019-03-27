#version 400

layout(points) in;
layout(triangle_strip, max_vertices = 15) out;

in vData
{
    vec3 position;
} vertices[];

out fData
{
    vec3 position;
} frag;

struct Metaball
{
    vec3 position;
    float radius;
};
uniform Metaball metaball;
uniform float voxelHalfLength;
uniform float isolevel;
uniform mat4 MVP;

// uniform isampler2D edgeTexture;
uniform isampler2D triTexture;

//Get triangle table value
int triTableValue(int i, int j){
    return texelFetch(triTexture, ivec2(j, i), 0).r;
}

//Compute interpolated vertex along an edge
vec3 VertexInterp(float isolevel, vec3 v0, vec3 v1,float l0, float l1){
    return mix(v0, v1, 0.5);
}

void main() {

    // define eight corner vertices for the cube (voxel)
    vec3 corners[8];
    corners[0] = vertices[0].position + vec3(-voxelHalfLength, -voxelHalfLength, +voxelHalfLength);
    corners[1] = vertices[0].position + vec3(+voxelHalfLength, -voxelHalfLength, +voxelHalfLength);
    corners[2] = vertices[0].position + vec3(+voxelHalfLength, -voxelHalfLength, -voxelHalfLength);
    corners[3] = vertices[0].position + vec3(-voxelHalfLength, -voxelHalfLength, -voxelHalfLength);
    corners[4] = vertices[0].position + vec3(-voxelHalfLength, +voxelHalfLength, +voxelHalfLength);
    corners[5] = vertices[0].position + vec3(+voxelHalfLength, +voxelHalfLength, +voxelHalfLength);
    corners[6] = vertices[0].position + vec3(+voxelHalfLength, +voxelHalfLength, -voxelHalfLength);
    corners[7] = vertices[0].position + vec3(-voxelHalfLength, +voxelHalfLength, -voxelHalfLength);

    float values[8];
    for(int i = 0; i < 8; ++i)
    {
        values[i] = distance(metaball.position, corners[i]);
    }

    // find the suitable look-up cube
    int cubeindex = 0;
    if (values[0] < isolevel) cubeindex |= 1;
    if (values[1] < isolevel) cubeindex |= 2;
    if (values[2] < isolevel) cubeindex |= 4;
    if (values[3] < isolevel) cubeindex |= 8;
    if (values[4] < isolevel) cubeindex |= 16;
    if (values[5] < isolevel) cubeindex |= 32;
    if (values[6] < isolevel) cubeindex |= 64;
    if (values[7] < isolevel) cubeindex |= 128;

    vec3 vertlist[12];

    /* Cube is entirely in/out of the surface */
    if (cubeindex == 0 || cubeindex == 255)
        return;

    /* Find the vertices where the surface intersects the cube */
    vertlist[0]  = VertexInterp(isolevel,corners[0],corners[1],values[0],values[1]);
    vertlist[1]  = VertexInterp(isolevel,corners[1],corners[2],values[1],values[2]);
    vertlist[2]  = VertexInterp(isolevel,corners[2],corners[3],values[2],values[3]);
    vertlist[3]  = VertexInterp(isolevel,corners[3],corners[0],values[3],values[0]);
    vertlist[4]  = VertexInterp(isolevel,corners[4],corners[5],values[4],values[5]);
    vertlist[5]  = VertexInterp(isolevel,corners[5],corners[6],values[5],values[6]);
    vertlist[6]  = VertexInterp(isolevel,corners[6],corners[7],values[6],values[7]);
    vertlist[7]  = VertexInterp(isolevel,corners[7],corners[4],values[7],values[4]);
    vertlist[8]  = VertexInterp(isolevel,corners[0],corners[4],values[0],values[4]);
    vertlist[9]  = VertexInterp(isolevel,corners[1],corners[5],values[1],values[5]);
    vertlist[10] = VertexInterp(isolevel,corners[2],corners[6],values[2],values[6]);
    vertlist[11] = VertexInterp(isolevel,corners[3],corners[7],values[3],values[7]);

    // push vertex to primitive
    int i = 0;
    frag.position = vec3(distance(metaball.position, corners[0]));
    for(int i = 0; triTableValue(cubeindex, i) != -1; i += 3)
    {
        gl_Position = MVP * vec4(vertlist[triTableValue(cubeindex, i)], 1.0);
        EmitVertex();
        gl_Position = MVP * vec4(vertlist[triTableValue(cubeindex, i+1)], 1.0);
        EmitVertex();
        gl_Position = MVP * vec4(vertlist[triTableValue(cubeindex, i+2)], 1.0);
        EmitVertex();
    }

    EndPrimitive();

}