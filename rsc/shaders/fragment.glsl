#version 330 core

in vec3 normal;
in vec3 toLight;
in float depth;

uniform sampler2D tex;
uniform vec3 toCamera;
uniform float orientationExp;
uniform bool useOrientation;
uniform float zmin;
uniform float depthScale;

out vec4 fragColor;

void main()
{
	vec2 texCoord;
	// diffuse shading.
	texCoord.x = max(dot(normal, toLight), 0);

	if (useOrientation)
	{
		// orientaion based attribute mapping.
		texCoord.y = pow(max(dot(normal, normalize(toCamera)), 0), orientationExp);
	}
	else
	{
		// use depth based mapping
		//float depth = length(toCamera);
		float zmax = depthScale * zmin;
		texCoord.y = 1 - log(depth/zmin) / log(zmax / zmin);
	}
	fragColor = texture(tex, texCoord);
}
