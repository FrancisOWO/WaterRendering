#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 lightColor;
uniform sampler2D textureMap;

void main()
{
	vec3 color = lightColor * vec3(texture(textureMap, TexCoords));
	FragColor = vec4(color, 1.0);
}
