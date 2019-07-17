float3 randVector(float n);

float3 randVector(float n) {
	float w;
	float x = fract(sin(n) * 43758.5453123, &w);
	float y = fract(sin(x) * 43758.5453123, &w);
	float z = fract(sin(y) * 43758.5453123, &w);
	return (float3)(x, y, z);
}

kernel void sphereSpreader(global float* out) {
	int i = get_global_id(0);
	float3 v = randVector(float(i + 1) / get_global_size(0));
	out[i * 3] = v.x;
	out[i * 3 + 1] = v.y;
	out[i * 3 + 2] = v.z;
}
