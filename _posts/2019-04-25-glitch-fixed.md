---
layout: post
title: Glitch Fixed!
---

Both of us are exhaused with the unexplainable glitch, so we took a long break during Easter. When we came back from the holiday, Alex discovered what causes this strange behaviour. What a relief! Here's how Alex explained what happened:

![]({{ site.baseurl }}/images/fixed-1.png)

![]({{ site.baseurl }}/images/fixed-2.png)

Apparently the memory space on GPU is not initialized, and it could be containing non-zero value. While the marching cubes algorithm doing a summation operation for calculating the isovalues, it was added on top of a non-zero value. The reason why it works on my Macbook Pro laptop but not Alex's Linux system may because of the Macbook hardware took care of it. Lesson learned here!

![]({{ site.baseurl }}/images/phong-1.png)
![]({{ site.baseurl }}/images/phong-2.png)

Alex also did some color shading on the fragment shader. He used the basic Blinn-Phong shading model on the metaballs. He then tweaks the grid resolution to make the surface more smooth.

```glsl
in fData{
    vec3 normal;
} frag;
out vec4 frag_colour;

uniform vec3 viewVector;

void main() {
    // Phong lightning model
    vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));
    vec3 reflectionDir = 2*dot(frag.normal, lightDir)*frag.normal - lightDir;

    vec3 ambientColor = vec3(0.2, 0.2, 0.2);
    vec3 diffuseColor = vec3(0.4, 0.5, 0.2);
    vec3 specularColor = vec3(0.8, 0.8, 0.8);
    
    float shininess = 7;

    vec3 finalColor =
        ambientColor
        + diffuseColor * max(0, dot(-frag.normal, lightDir))
        + specularColor * pow(max(dot(reflectionDir, normalize(viewVector)),0), shininess);

	frag_colour = vec4(finalColor, 1.0);
}
```

So now we both have the marching cubes commit running on our laptops and the metaballs are now beautifully colored. It is time to more on to actual physics simulation.
