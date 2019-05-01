#version 400

in vec3 vp;
out vData
{
	vec3 position;
} vertex;

uniform mat4 MVP;  // uniform = value stays the same for the whole mesh

void main() {
	//	Do nothing, pass the grid position to geometry shader
	vertex.position = vp;
}