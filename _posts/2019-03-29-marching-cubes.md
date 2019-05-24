---
layout: post
title: Marching Cubes
---

While doing some online research on marching cubes in OpenGL, I realised most of them are implemented in CUDA or other GPU programming language. Only some of them like the one on [icare3d.org](http://www.icare3d.org/codes-and-projects/codes/opengl_geometry_shader_marching_cubes.html) uses geometry shader to compute the marching cubes algorithm. So I gave it a try and followed his implementation.

I carefully stepped through his implementation and recreated in the geometry shader. And I set the position and radius of the metaballs one-by-one by passing them as some separate uniforms. However, this weird glitch occurs when I set the metaball count in the for-loop as a uniform. 

![]({{ site.baseurl }}/images/marching-cubes-glitch.png)

This was later discovered that OpenGL 4.1 does not support variables in a loop inside any shaders. We could only define the number of iterations by a constant value. So for now, we hard-code a constant for the inverse quadratic calculation first. And it works fine:

![]({{ site.baseurl }}/images/marching-cubes-working.png)

Two metaballs are merging like how they should behave. Great! I also calculated the surface normal for later shading uses. The above image shows the surface normal in RGB colors.