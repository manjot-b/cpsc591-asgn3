#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform vec3 lightPosition;
uniform vec3 toCamera;

out vec3 normal;
out vec3 toLight;
out float depth;

void main()
{
	vec4 worldPosition = model * vec4(inPosition, 1.0f);
    gl_Position = perspective * view * worldPosition;
	normal = normalize((model * vec4(inNormal, 0.0f)).xyz);
	toLight = normalize(lightPosition - worldPosition.xyz);
	depth = length(toCamera - worldPosition.xyz);
}
