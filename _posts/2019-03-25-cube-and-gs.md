---
layout: post
title: Perspective and Geometry Shader
---

Now we have some perspective view on our scene. Alex has placed a nice little cube in the scene for demonstrating our 3D world.

![]({{ site.baseurl }}/images/cube.png)

With that being setup, we can now test if the geometry shader is working or not. So I created a geometry shader program in between vertex and fragment shaders. Since this is the first time for me to deal with geometry shader, I would like to play around and see how things change.

```glsl
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void main() {   
    for(int i = 0; i < gl_in.length(); ++i)
    {
        gl_Position = gl_in[i].gl_Position + vec4(i, i, i, 0.0f);
        EmitVertex();
    }
    EndPrimitive();
} 
```

The above code is just messing around with the vertex position output from the vertex shader. It adds some arbitary numbers to the original vertex position. The resulting cube now looks like it is disintegrating / exploding.

![]({{ site.baseurl }}/images/cube-gs.png)