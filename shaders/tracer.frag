#version 330

uniform sampler3D tex;
uniform float march;
uniform float detail;

in vec3 worldVertex;
in vec3 worldCam;

out vec4 outputColor;

void main() {
	vec3 eyeDirection = normalize(worldVertex - worldCam);
	vec3 ray = 0.5*(worldVertex + vec3(1, 1, 1));
	vec4 color = vec4(0.01,0.01,0.02, 0.02);
	float marchMod = max(0.001, march) * detail;
	while (ray.x >= 0 && ray.x <= 1 && ray.y >= 0 && ray.y <= 1 && ray.z >= 0 && ray.z <= 1) {
		ray = ray + (marchMod*eyeDirection);
		vec4 colorSample = texture(tex, ray);
		
		color = color + vec4(colorSample.rgb*colorSample.a, colorSample.a);
	}
	outputColor = vec4(normalize(color.rgb), color.a);
}
