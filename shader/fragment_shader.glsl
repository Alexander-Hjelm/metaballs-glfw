#version 400

in fData
{
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

    // TODO: normal seems to be inverted, coming from the geometry shader
    vec3 finalColor =
        ambientColor
        + diffuseColor * max(0, dot(-frag.normal, lightDir))
        + specularColor * pow(max(dot(reflectionDir, normalize(viewVector)),0), shininess);

	frag_colour = vec4(finalColor, 1.0);
}
