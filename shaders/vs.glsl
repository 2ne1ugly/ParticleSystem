#version 410

layout(location = 0) in vec3 posVsIn;
out vec3 pos;

uniform mat4x4 viewProj;

void main() {
	//particles are already in world space.
	gl_Position = viewProj * vec4(posVsIn, 1.0);
	pos = gl_Position.xyz;
}
