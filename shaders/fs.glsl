#version 410

out vec3 color;
in vec3 pos;

void main() {

	color = vec3(0.0, 1.0,1.0) * (1 - pos.z) + vec3(1.0, 1.0, 1.0) * (pos.z);
}