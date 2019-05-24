---
layout: post
title: Making Voxel Cubes
---

More classes are being added to our program. The Metaball class is used to define the position and radius of a metaball in a 3D space. And we added some keyboard controls over our first metaball.

The metaball can now be controlled with WASD for XZ-plane movement, Shift and Space for Y-axis movement. And we programmed the metaball to wrap itself if it ever goes out of the boundary.

For rendering the metaball, I programmed it in a way that it renders a solid box if a point in the grid falls within the metaball's radius. Otherwise, do not show anything. This can be achieved by the following shader code:

```glsl
layout(points) in;
layout(triangle_strip, max_vertices = 14) out;

in vData{
    vec3 position;
} vertices[];

out fData{
    vec3 position;
} frag;

struct Metaball{
    vec3 position;
    float radius;
};
uniform Metaball metaball;
uniform float voxelHalfLength;
uniform mat4 MVP;

const int INDICES[14] = int[14]( 4, 3, 7, 8, 5, 3, 1, 4, 2, 7, 6, 5, 2, 1 );    //  minimalistic way to define a unit cube

void main() {
    // voxel is not inside the sphere
    if(distance(metaball.position, vertices[0].position) > metaball.radius)
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
```

The metaball is taking its shape now!

![]({{ site.baseurl }}/images/voxel-ball.png)

Alex has further made it to be behaving more like a real metaball by implementing a quadratic metasurface model into the geometry shader. It looks something like this:

![]({{ site.baseurl }}/images/voxel-connect.png)

Pretty cool! For the next goal, instead of rendering blocky surfaces, we would like to implement the marching cubes algorithm inside the geometry shader. 