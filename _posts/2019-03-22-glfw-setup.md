---
layout: post
title: GLFW Setup
---

This is the start of our project. The base code was first written by Alex on his Linux system. He linked all necessary libraries into the repository and created a makefile for program compilation.

We decided to use [GLFW](https://github.com/glfw/glfw) as the window managing library and [GLAD](https://github.com/Dav1dde/glad) as the OpenGL API loader library. We will also use [GLM](https://github.com/g-truc/glm.git) as our maths library.

Here is our first windowed application running:

![]({{ site.baseurl }}/images/glfw.png)

After testing the program would work on both MacOS and Linux system, we decided to try rendering a simple triangle next. Meanwhile, we can do some project hierarchy cleaning (.gitignore) and constructing our project specification. 