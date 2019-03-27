#version 400

in fData
{
    vec3 position;
} frag;
out vec4 frag_colour;

void main() {

	// frag_colour = vec4(frag.position, 1.0);
	frag_colour = vec4(frag.position, 1.0);
  
}