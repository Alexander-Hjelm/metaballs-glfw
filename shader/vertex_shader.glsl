#version 400

in vec3 vp;
out vData
{
	vec3 position;
} vertex;

uniform mat4 MVP;  // uniform = value stays the same for the whole mesh

void main() {

  vertex.position = vp;
  gl_Position = MVP * vec4(vp, 1.0);

}