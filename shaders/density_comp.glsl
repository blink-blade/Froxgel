layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout(std430, binding = 0) buffer DensityBuffer {
    float density[];
};

uniform int gridSize;

uint index3D(uint x, uint y, uint z)
{
    return x + y * gridSize + z * gridSize * gridSize;
}

void main()
{
    uvec3 id = gl_GlobalInvocationID;
    uint index = index3D(id.x, id.y, id.z);
    density[index] = layeredNoise3D(id.x, id.y, id.z, 3, 0.05) * 10;
}